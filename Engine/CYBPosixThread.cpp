//! @file CYBPosixThread.cpp Implements CYB::Platform::Thread for Posix
#include "CYB.hpp"


CYB::Platform::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FRunning(false)
{
	ThreadData Data{ &AThreadable, &FRunning };
	const auto Result(Posix::pthread_create(&FThread, nullptr, ThreadProc, &Data));
	if (Result != 0) {
		//! @todo Log error
		throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
	}
	while (!FRunning.load(std::memory_order_release))
		Platform::Thread::Yield();
}

void* CYB::Platform::Implementation::Thread::ThreadProc(void* const AThreadData) {
	std::atomic_bool* const RunningAtomic(static_cast<volatile ThreadData*>(AThreadData)->FRunningPointer);
	API::Threadable* Threadable(static_cast<volatile ThreadData*>(AThreadData)->FThreadable);
	RunningAtomic->store(true, std::memory_order_release);
	try {
		try {
			Threadable->BeginThreadedOperation();
		}
		catch (CYB::Exception::Base AException) {
			//! @todo Log error
			static_cast<void>(AException);
		}
	}
	catch (...) {}
	RunningAtomic->store(false, std::memory_order_release);
	return nullptr;
}
bool CYB::Platform::Thread::IsFinished(void) const {
	return !FRunning.load(std::memory_order_relaxed);
}

void CYB::Platform::Thread::Wait(void) const {
	Implementation::Posix::pthread_join(FThread, nullptr);
}