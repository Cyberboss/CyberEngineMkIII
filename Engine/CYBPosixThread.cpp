//! @file CYBPosixThread.cpp Implements CYB::Platform::Thread for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Implementation::Posix;

class PThreadLockGuard {
private:
	pthread_mutex_t* const FMutex;
public:
	PThreadLockGuard(pthread_mutex_t* const AMutex, const bool ADoLock) : FMutex(AMutex) { if(ADoLock) pthread_mutex_lock(AMutex); }
	~PThreadLockGuard() { pthread_mutex_unlock(FMutex); }
};

CYB::Platform::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FRunningLock(PTHREAD_MUTEX_INITIALIZER),
	FRunning(false)
{
	{
		ThreadData Data{ &AThreadable, &FRunningLock, &FRunning };

		if (pthread_mutex_init(&FRunningLock, nullptr) != 0)
			throw Exception::SystemData(Exception::SystemData::ErrorCode::MUTEX_INITIALIZATION_FAILURE);

		pthread_mutex_lock(&FRunningLock);

		std::atomic_thread_fence(std::memory_order_release);

		if (pthread_create(&FThread, nullptr, ThreadProc, &Data) != 0) {
			//! @todo Log error
			pthread_mutex_unlock(&FRunningLock);
			DestroyMutex();
			throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
		}

		PThreadLockGuard Lock(&FRunningLock, false);

		if (!FRunning.load(std::memory_order_release)) {
			do {
				Platform::Thread::Yield();
			} while (!FRunning.load(std::memory_order_relaxed));
		}
	}
}

CYB::Platform::Implementation::Thread::~Thread() {
	DestroyMutex();
}

void* CYB::Platform::Implementation::Thread::ThreadProc(void* const AThreadData) {
	auto& Data(*static_cast<volatile ThreadData*>(AThreadData));

	API::Threadable* const Threadable(Data.FThreadable);
	std::atomic_bool* const RunningAtomic(Data.FRunning);
	pthread_mutex_t* const RunningLock(Data.FRunningLock);

	RunningAtomic->store(true, std::memory_order_release);

	PThreadLockGuard Lock(RunningLock, true);
	try {
		Threadable->BeginThreadedOperation();
	}
	catch (CYB::Exception::Base AException) {
		//! @todo Log error
		static_cast<void>(AException);
	}
	catch (...) {
		//! @todo Log error
	}
	RunningAtomic->store(false, std::memory_order_release);
	return nullptr;
}

void CYB::Platform::Implementation::Thread::DestroyMutex(void) {
	if (pthread_mutex_destroy(&FRunningLock) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_DESTRUCTION_FAILURE);
}

bool CYB::Platform::Thread::IsFinished(void) const {
	return !FRunning.load(std::memory_order_relaxed);
}

void CYB::Platform::Thread::Wait(void) const {
	pthread_join(FThread, nullptr);
}

void CYB::Platform::Thread::Sleep(const unsigned int AMilliseconds) {
	usleep(AMilliseconds * 1000);
}

void CYB::Platform::Thread::Yield(void) {
	sched_yield();
}