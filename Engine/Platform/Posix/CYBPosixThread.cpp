#include "CYB.hpp"

using namespace CYB::Platform::Implementation::Posix;

namespace CYB {
	namespace Platform {
		namespace Implementation {
			class PThreadLockGuard {
			private:
				pthread_mutex_t* const FMutex;
			public:
				PThreadLockGuard(pthread_mutex_t* const AMutex, const bool ADoLock) : FMutex(AMutex) { if (ADoLock) Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_lock>(AMutex); }
				~PThreadLockGuard() { Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_unlock>(FMutex); }
			};
		};
	};
};

CYB::Platform::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FRunningLock(PTHREAD_MUTEX_INITIALIZER),
	FRunning(false)
{
	{
		auto& PThread(Core().FModuleManager.FPThread);
		ThreadData Data{ &AThreadable, &FRunningLock, &FRunning };

		try {
			if (PThread.Call<ModuleDefinitions::PThread::pthread_mutex_init>(&FRunningLock, nullptr) != 0)
				throw Exception::SystemData(Exception::SystemData::ErrorCode::MUTEX_INITIALIZATION_FAILURE);	//Throw for the specific reason
		} catch(CYB::Exception::SystemData AException) {	//But always translate
			if(AException.FErrorCode == Exception::SystemData::ErrorCode::MUTEX_INITIALIZATION_FAILURE)
				throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
			throw;
		}
		PThread.Call<ModuleDefinitions::PThread::pthread_mutex_lock>(&FRunningLock);

		std::atomic_thread_fence(std::memory_order_release);

		if (PThread.Call<ModuleDefinitions::PThread::pthread_create>(&FThread, nullptr, ThreadProc, &Data) != 0) {
			//! @todo Log error
			PThread.Call<ModuleDefinitions::PThread::pthread_mutex_unlock>(&FRunningLock);
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

	auto const Threadable(Data.FThreadable);
	auto const RunningAtomic(Data.FRunning);
	auto const RunningLock(Data.FRunningLock);

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
	if (Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_destroy>(&FRunningLock) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_DESTRUCTION_FAILURE);
}

bool CYB::Platform::Thread::IsFinished(void) const {
	return !FRunning.load(std::memory_order_relaxed);
}

void CYB::Platform::Thread::Wait(void) const {
	Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_join>(FThread, nullptr);
}

void CYB::Platform::Thread::Sleep(const unsigned int AMilliseconds) {
	Core().FModuleManager.FC.Call<ModuleDefinitions::LibC::usleep>(AMilliseconds * 1000);
}

void CYB::Platform::Thread::Yield(void) {
	Core().FModuleManager.FRT.Call<ModuleDefinitions::RT::sched_yield>();
}