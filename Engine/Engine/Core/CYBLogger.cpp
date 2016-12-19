//! @file CYBLogger.cpp Implements CYB::Engine::Logger
#include "CYB.hpp"

#include <cstdio>
#include <ctime>

CYB::Platform::System::File CYB::Engine::Logger::OpenFile(void) {
	Platform::System::Path LogPath(API::Path::SystemPath::TEMPORARY);

	auto Time(time(0));   // get time now
	auto Now = localtime(&Time);

	const int Year(Now->tm_year + 1900), Month(Now->tm_mon + 1), Day(Now->tm_mday), Hour(Now->tm_hour), Min(Now->tm_min), Sec(Now->tm_sec);

	char Buffer[50];
	const auto Length(sprintf(Buffer, u8"LogFile %d-%d-%d %d:%d:%d.txt", Year, Month, Day, Hour, Min, Sec));

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
		try {							//Yes, copy it, if it throws it won't be valid anymore
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
	FQueue(nullptr),
	FCancelled(false),
	FThread(nullptr)
{
}

CYB::Engine::Logger::~Logger() {
	CancelThreadedOperation();
	FThread->Wait();
	EmptyQueue();
}

void CYB::Engine::Logger::EmptyQueue(void) noexcept {

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

void CYB::Engine::Logger::Log(const API::String::CStyle& AMessage, const Level ALevel) noexcept {
	static_cast<void>(AMessage);
	static_cast<void>(ALevel);
}

const CYB::API::String::CStyle& CYB::Engine::Logger::CurrentLog(void) const noexcept {
	return FFile.GetPath()();
}