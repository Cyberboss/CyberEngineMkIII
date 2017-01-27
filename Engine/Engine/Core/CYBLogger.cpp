//! @file CYBLogger.cpp Implements CYB::Engine::Logger
#include "CYB.hpp"

#include <cstdio>
#include <ctime>
#include <cstring>

CYB::API::String::Dynamic CYB::Engine::Logger::TimeString(const int AHour, const int AMinute, const int ASecond, const bool AColons) {
	char Buffer[50];
	memset(Buffer, 0, 50);
	int Length;
	if (AColons)
		Length = sprintf(Buffer, u8"[%02d:%02d:%02d]", AHour, AMinute, ASecond);
	else
		Length = sprintf(Buffer, u8"%02d-%02d-%02d", AHour, AMinute, ASecond);

	API::Assert::LessThan(-1, Length);
	API::Assert::LessThan(Length, 50);

	return API::String::Dynamic(API::String::Static(Buffer));
}

CYB::API::String::Dynamic CYB::Engine::Logger::TimeString(const bool AColons) {
	auto Time(time(0));   // get time now
	auto Now(localtime(&Time));
	const auto Hour(Now->tm_hour), Min(Now->tm_min), Sec(Now->tm_sec);
	return TimeString(Hour, Min, Sec, AColons);
}

CYB::Platform::System::File CYB::Engine::Logger::OpenFileImpl(const Platform::System::Path& ABaseLogPath) {
	try {

		auto Time(time(0));   // get time now
		auto Now(localtime(&Time));

		const int Year(Now->tm_year + 1900), Month(Now->tm_mon + 1), Day(Now->tm_mday), Hour(Now->tm_hour), Min(Now->tm_min), Sec(Now->tm_sec);

		char Buffer[50];
		memset(Buffer, 0, 50);
		const auto Length(sprintf(Buffer, u8"Engine Log %d-%d-%d %s.txt", Year, Month, Day, TimeString(Hour, Min, Sec, false).CString()));

		API::Assert::LessThan(-1, Length);
		API::Assert::LessThan(Length, 50);

		API::String::Static AsStatic(Buffer);
		API::String::UTF8 Formatted(AsStatic);

		auto LogPath(ABaseLogPath);
		LogPath.Append(std::move(Formatted), false, false);

		return Platform::System::File(std::move(LogPath), API::File::Mode::WRITE, API::File::Method::CREATE);
	}
	catch (CYB::Exception::SystemData& AException) {
		if (AException.FErrorCode != CYB::Exception::SystemData::FILE_EXISTS)
			throw;
	}
	Platform::System::Thread::Sleep(1000);
	return OpenFileImpl(ABaseLogPath);
}
CYB::Platform::System::File CYB::Engine::Logger::OpenFile(void) {
	//All allocation is done using the Logger's Allocator
	Platform::System::Path ThePath(API::Path::SystemPath::TEMPORARY);
	return OpenFileImpl(ThePath);
}

CYB::Engine::Logger::Logger(API::Logger& AEmergencyLogger) :
	FHeap(Parameters::LOGGER_HEAP_INITIAL_COMMIT_SIZE),
	FContext(FHeap, AEmergencyLogger, true),
	FFile(OpenFile()),
	FQueueHead(nullptr),
	FQueueTail(nullptr),
	FThread(nullptr),
	FCancelled(false),
	FDevLog(
#ifdef DEBUG
		true
#else
		false
#endif
	)
{
	FQueueHead.reset(static_cast<Allocator&>(Context::GetContext().FAllocator).RawObject<LogEntry>());
	FQueueHead->FLevel = Level::INFO;
	FQueueHead->FMessage = FormatLogMessage(API::String::Static(u8"CyberEngineMkIII logger started..."), Level::INFO);

	FQueueTail = FQueueHead.get();

	FThread = FContext.FAllocator.ConstructObject<Platform::System::Thread, API::Interop::NullConstructor>(SelfAsThreadable());
}

CYB::Engine::Logger::~Logger() {
	Log(API::String::Static(u8"Logger shutting down"), Level::INFO);
	FContext.MakeCurrent();	//At this point, Core's Context is dead and we won't use it again
	//using PushContext won't help as we'll try to dealloc strings in FFile with the wrong allocator
	CancelThreadedOperation();
	FThread().Wait();
	EmptyQueue();
}

void CYB::Engine::Logger::LogShutdownForEntry(API::UniquePointer<LogEntry>&& AEntry, API::Logger& AEmergency) noexcept {
	//Emergency log the message
	auto& Message(AEntry->FMessage);
	const auto ByteIndex(Message.IndexOfByte(':', 3) + 1);	//+1 to comp for the space
	API::Assert::LessThan(0, ByteIndex);
	API::Assert::LessThan(ByteIndex, Message.RawLength());
	AEmergency.Log(API::String::Static(Message.CString() + ByteIndex), AEntry->FLevel);
	if (AEntry->FNext != nullptr)
		LogShutdownForEntry(std::move(AEntry->FNext), AEmergency);
}

void CYB::Engine::Logger::EmptyQueue(void) {

	API::UniquePointer<LogEntry> Queue, NextNode;
	{
		API::LockGuard Lock(FQueueLock);
		Queue = std::move(FQueueHead);
		FQueueTail = nullptr;
	}

	API::LockGuard LockFile(FFileLock);
	for (; Queue.get() != nullptr; Queue = std::move(NextNode)) {
		NextNode = std::move(Queue->FNext);
		const auto Len(static_cast<unsigned int>(Queue->FMessage.RawLength()));
		auto Written(0ULL);
		do {
			const auto CurrentWrite(FFile.Write(Queue->FMessage.CString() + Written, Len - Written));
			if (CurrentWrite == 0) {
				//CurrentContext will most certainly be FContext at this point
				//but just in case we add some weird behaviour overrides in the future....
				auto& EmergencyLogger(Context::GetContext().FLogger);
				EmergencyLogger.SetDebugLogging(true);
				EmergencyLogger.Log(API::String::Static(u8"Failed to write to primary log. Message follows:"), Level::ERR);
				EmergencyLogger.Log(Queue->FMessage, Queue->FLevel);
				if (NextNode != nullptr) {
					EmergencyLogger.Log(API::String::Static(u8"Remaining entries follow:"), Level::INFO);
					LogShutdownForEntry(std::move(NextNode), EmergencyLogger);
				}
				throw CYB::Exception::SystemData(CYB::Exception::SystemData::STREAM_NOT_WRITABLE);
			}
			Written += CurrentWrite;
		} while (Written < Len);
		API::Assert::Equal<unsigned long long>(Written, Len);
	}
}

CYB::API::Threadable& CYB::Engine::Logger::SelfAsThreadable(void) noexcept {
	return *this;
}

void CYB::Engine::Logger::BeginThreadedOperation(void) {
	PushContext Push(FContext);
	while (!FCancelled.load(std::memory_order_relaxed)) {
		EmptyQueue();
		Platform::System::Thread::Sleep(1);
	}
}

void CYB::Engine::Logger::CancelThreadedOperation(void) {
	FCancelled.store(true, std::memory_order_relaxed);
}

CYB::API::String::Dynamic CYB::Engine::Logger::FormatLogMessage(const API::String::CStyle& AMessage, const Level ALevel) {
	const char* LevelString;
	switch (ALevel) {
	case Level::DEV:
		LevelString = u8": Debug: ";
		break;
	case Level::INFO:
		LevelString = u8": Info: ";
		break;
	case Level::WARN:
		LevelString = u8": Warning: ";
		break;
	case Level::ERR:
		LevelString = u8": ERROR: ";
		break;
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
	return TimeString(true) + API::String::Static(LevelString) + AMessage;
}

void CYB::Engine::Logger::Log(const API::String::CStyle& AMessage, const Level ALevel) {
	if (ALevel != Level::DEV || FDevLog.load(std::memory_order_relaxed)) {
		PushContext Push(FContext);	//Use ourselves for allocation
		bool CritFail(false);
		for (auto I(0U); I < (Parameters::LOGGER_HEAP_RETRY_COUNT + 1) && !CritFail; ++I) {
			try {
				API::UniquePointer<LogEntry> Entry(static_cast<Allocator&>(Context::GetContext().FAllocator).RawObject<LogEntry>());
				Entry->FMessage = API::String::Dynamic(u8"\n") + FormatLogMessage(AMessage, ALevel);
				Entry->FLevel = ALevel;

				API::LockGuard Lock(FQueueLock);
				if (FQueueTail != nullptr)
					FQueueTail->FNext = std::move(Entry);
				else 
					FQueueHead = std::move(Entry);
				FQueueTail = Entry.get();
				break;
			}
			catch (CYB::Exception::SystemData& AException) {
				API::Assert::Equal<unsigned int>(AException.FErrorCode, CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE);
				try {
					//Empty the queue, free the memory, and try again
					EmptyQueue();
				}
				catch (CYB::Exception::SystemData& AInnerException) {
					API::Assert::Equal<unsigned int>(AInnerException.FErrorCode, CYB::Exception::SystemData::STREAM_NOT_WRITABLE);
					//Now give up
					CritFail = true;
				}
			}
		}
	}
}

void CYB::Engine::Logger::Flush(void) const noexcept {
	do {
		{	//lock in reverse order so we know there are no dangling entry references
			API::LockGuard FileLock(FFileLock), QueueLock(FQueueLock);
			if (FQueueHead == nullptr)
				break;
		}
		Platform::System::Thread::Sleep(1);
	} while (true);
}

const CYB::API::String::CStyle& CYB::Engine::Logger::CurrentLog(void) const noexcept {
	return FFile.GetPath()();
}

void CYB::Engine::Logger::SetDebugLogging(const bool AEnable) noexcept {
	FDevLog.store(AEnable, std::memory_order_release);
}