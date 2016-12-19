//! @file CYBLogger.cpp Implements CYB::Engine::Logger
#include "CYB.hpp"

CYB::Engine::Logger::Logger() :
	FHeap(Parameters::LOGGER_HEAP_INITIAL_COMMIT_SIZE),
	FContext(FHeap, true),
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

const CYB::API::Path& CYB::Engine::Logger::CurrentLog(void) const noexcept {
	return FFile.GetPath();
}