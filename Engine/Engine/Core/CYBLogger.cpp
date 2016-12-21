//! @file CYBLogger.cpp Implements CYB::Engine::Logger
#include "CYB.hpp"

#include <cstdio>
#include <ctime>

CYB::API::String::Dynamic CYB::Engine::Logger::TimeString(const int AHour, const int AMinute, const int ASecond, const bool AColons) {
	char Buffer[50];

	int Length;
	if (AColons)
		Length = sprintf(Buffer, u8"[%02d:%02d:%02d]", AHour, AMinute, ASecond);
	else
		Length = sprintf(Buffer, u8"%02d-%02d-%02d", AHour, AMinute, ASecond);

	API::Assert::LessThan(-1, Length);
	API::Assert::LessThan(Length, 50);

#ifdef TARGET_OS_WINDOWS
	//Code analysis is stupid
	__assume(-1 < Length && Length < 50);
#endif
	Buffer[Length] = 0;

	return API::String::Dynamic(API::String::Static(Buffer));
}

CYB::API::String::Dynamic CYB::Engine::Logger::TimeString(const bool AColons) {
	auto Time(time(0));   // get time now
	auto Now = localtime(&Time);
	const auto Hour(Now->tm_hour), Min(Now->tm_min), Sec(Now->tm_sec);
	return TimeString(Hour, Min, Sec, AColons);
}

CYB::Platform::System::File CYB::Engine::Logger::OpenFile(void) {
	//All allocation is done using the Logger's Allocator
	Platform::System::Path LogPath(API::Path::SystemPath::TEMPORARY);

	auto Time(time(0));   // get time now
	auto Now = localtime(&Time);

	const int Year(Now->tm_year + 1900), Month(Now->tm_mon + 1), Day(Now->tm_mday), Hour(Now->tm_hour), Min(Now->tm_min), Sec(Now->tm_sec);

	char Buffer[50];
	const auto Length(sprintf(Buffer, u8"Engine Log %d-%d-%d %s.txt", Year, Month, Day, TimeString(Hour, Min, Sec, false).CString()));

	API::Assert::LessThan(-1, Length);
	API::Assert::LessThan(Length, 50);

#ifdef TARGET_OS_WINDOWS
	//Code analysis is stupid
	__assume(-1 < Length && Length < 50);
#endif

	Buffer[Length] = 0;
	API::String::Static AsStatic(Buffer);
	API::String::UTF8 Formatted(AsStatic);

	LogPath.Append(std::move(Formatted), false, false);

	do {
		try {							//Yes, copy LogPath, if it throws it won't be valid anymore
			return Platform::System::File(LogPath, API::File::Mode::WRITE, API::File::Method::CREATE);
		}
		catch (CYB::Exception::SystemData& AException) {
			if (AException.FErrorCode != CYB::Exception::SystemData::FILE_EXISTS)
				throw;
		}
	} while (true);
}

CYB::Engine::Logger::Logger(API::Logger& AEmergencyLogger) :
	FHeap(Parameters::LOGGER_HEAP_INITIAL_COMMIT_SIZE),
	FContext(FHeap, AEmergencyLogger, true),
	FFile(OpenFile()),
	FQueueHead(nullptr),
	FQueueTail(nullptr),
	FCancelled(false),
	FThread(nullptr)
{
	auto InitEntry(static_cast<Allocator&>(Context::GetContext().FAllocator).RawObject<LogEntry>());
	InitEntry->FLevel = Level::INFO;
	InitEntry->FMessage = FormatLogMessage(API::String::Static("CyberEngineMkIII logging started..."), Level::INFO);
	InitEntry->FNext = nullptr;

	FQueueHead = InitEntry;
	FQueueTail = InitEntry;

	EmptyQueue();
	
	FThread = FContext.FAllocator.NewObject<Platform::System::Thread, API::Interop::NullConstructor>(static_cast<API::Threadable&>(*this));
}

CYB::Engine::Logger::~Logger() {
	CancelThreadedOperation();
	FThread().Wait();
	EmptyQueue();
}

void CYB::Engine::Logger::EmptyQueue(void) {
	LogEntry* Queue, *NextNode;
	{
		API::LockGuard Lock(FQueueLock);
		Queue = FQueueHead;
		FQueueHead = nullptr;
		FQueueTail = nullptr;
	}

	class CleanLogEntry {
	private:
		LogEntry* const FLogEntry;
	public:
		CleanLogEntry(LogEntry* const ALogEntry, const bool ARecursiveShutdown) :
			FLogEntry(ALogEntry)
		{
			if (ARecursiveShutdown) {
				//Emergency log the message
				Context::GetContext().FLogger.Log(ALogEntry->FMessage, ALogEntry->FLevel);
				if (FLogEntry->FNext != nullptr)
					CleanLogEntry(FLogEntry->FNext, true);
			}
		}
		~CleanLogEntry() {
			Context::GetContext().FAllocator.DeleteObject<LogEntry>(FLogEntry);
		}
	};


	API::LockGuard LockFile(FFileLock);
	for (auto Node(Queue); Node != nullptr; Node = NextNode) {
		NextNode = Node->FNext;
		CleanLogEntry Cleanup(Node, false);
		const auto Len(static_cast<unsigned int>(Node->FMessage.RawLength()));
		auto Written(0ULL);
		do {
			const auto CurrentWrite(FFile.Write(Node->FMessage.CString() + Written, Len - Written));
			if (CurrentWrite == 0) {
				//CurrentContext will most certainly be FContext at this point
				//but just in case we add some weird behaviour overrides in the future....
				auto& EmergencyLogger(Context::GetContext().FLogger);
				EmergencyLogger.Log(API::String::Static(u8"Failed to write to primary log. Message follows:"), Level::ERR);
				EmergencyLogger.Log(Node->FMessage, Node->FLevel);
				if (NextNode != nullptr) {
					EmergencyLogger.Log(API::String::Static(u8"Remaining entries follow:"), Level::ERR);
					CleanLogEntry(NextNode, true);
				}
				throw CYB::Exception::SystemData(CYB::Exception::SystemData::STREAM_NOT_WRITABLE);
			}
			Written += CurrentWrite;
		} while (Written < Len);
		API::Assert::Equal<unsigned long long>(Written, Len);
	}
}

void CYB::Engine::Logger::BeginThreadedOperation(void) {
	FContext.MakeCurrent();
	while (!FCancelled.load(std::memory_order_relaxed)) {
		EmptyQueue();
		Platform::System::Thread::Sleep(1);
	}
}

void CYB::Engine::Logger::CancelThreadedOperation(void) {
	FCancelled.store(true, std::memory_order_relaxed);
}

CYB::API::String::Dynamic CYB::Engine::Logger::FormatLogMessage(const API::String::CStyle& AMessage, const Level ALevel) {
	char* LevelString;
	switch (ALevel) {
	case Level::DEV:
		LevelString = ": Debug: ";
		break;
	case Level::INFO:
		LevelString = ": Info: ";
		break;
	case Level::WARN:
		LevelString = ": Warning: ";
		break;
	case Level::ERR:
		LevelString = ": ERROR: ";
		break;
	default:
		throw CYB::Exception::Violation(CYB::Exception::Violation::INVALID_ENUM);
	}
	return TimeString(true) + API::String::Static(LevelString) + AMessage;
}

void CYB::Engine::Logger::Log(const API::String::CStyle& AMessage, const Level ALevel) {
#ifndef DEBUG
	if (ALevel == Level::DEV)
		return;
#endif

	PushContext Push(FContext);	//Use ourselves for allocation
	bool CritFail(false);
	while (!CritFail) {
		try {
			auto Entry(static_cast<Allocator&>(Context::GetContext().FAllocator).RawObject<LogEntry>());
			Entry->FNext = nullptr;
			Entry->FMessage = API::String::Dynamic(u8"\n") + FormatLogMessage(AMessage, ALevel);
			Entry->FLevel = ALevel;

			API::LockGuard Lock(FQueueLock);
			if (FQueueTail != nullptr)
				FQueueTail->FNext = Entry;
			else
				FQueueHead = Entry;
			FQueueTail = Entry;
			break;
		}
		catch (CYB::Exception::SystemData& AException) {
			API::Assert::Equal<unsigned int>(AException.FErrorCode, CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE);
			try {
				//Empty the queue, free the memory, and try again
				EmptyQueue();
			}
			catch(CYB::Exception::SystemData& AInnerException){
				API::Assert::Equal<unsigned int>(AInnerException.FErrorCode, CYB::Exception::SystemData::STREAM_NOT_WRITABLE);
				//Now give up
				CritFail = true;
			}
		}
	}
}

void CYB::Engine::Logger::Flush(void) const noexcept {
	do {
		API::LockGuard FileLock(FFileLock), QueueLock(FQueueLock);
		if (FQueueHead == nullptr)
			break;
		Platform::System::Thread::Sleep(1);
	} while (true);
}

const CYB::API::String::CStyle& CYB::Engine::Logger::CurrentLog(void) const noexcept {
	return FFile.GetPath()();
}