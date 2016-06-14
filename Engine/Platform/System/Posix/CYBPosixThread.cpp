#include "CYB.hpp"

using namespace CYB::Platform::Posix;

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class PThreadLockGuard {
				private:
					pthread_mutex_t* const FMutex;
				public:
					PThreadLockGuard(pthread_mutex_t* const AMutex, const bool ADoLock) : FMutex(AMutex) { if (ADoLock) Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_lock>(AMutex); }
					~PThreadLockGuard() { Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_unlock>(FMutex); }
				};
			};
		};
	};
};

CYB::Platform::System::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FRunningLock(PTHREAD_MUTEX_INITIALIZER),
	FRunning(false)
{
	{
		auto& PThread(Core().FModuleManager.FPThread);
		ThreadData Data{ &AThreadable, &FRunningLock, &FRunning };

		try {
			if (PThread.Call<Modules::PThread::pthread_mutex_init>(&FRunningLock, nullptr) != 0)
				throw Exception::SystemData(Exception::SystemData::ErrorCode::MUTEX_INITIALIZATION_FAILURE);	//Throw for the specific reason
		} catch(CYB::Exception::SystemData AException) {	//But always translate
			API::Assert::Equal(AException.FErrorCode, static_cast<unsigned int>(Exception::SystemData::ErrorCode::MUTEX_INITIALIZATION_FAILURE));
			throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
		}	//optimizer should take care of this
		PThread.Call<Modules::PThread::pthread_mutex_lock>(&FRunningLock);

		std::atomic_thread_fence(std::memory_order_release);

		if (PThread.Call<Modules::PThread::pthread_create>(&FThread, nullptr, ThreadProc, &Data) != 0) {
			PThread.Call<Modules::PThread::pthread_mutex_unlock>(&FRunningLock);
			DestroyMutex();
			throw Exception::SystemData(Exception::SystemData::ErrorCode::THREAD_CREATION_FAILURE);
		}

		PThreadLockGuard Lock(&FRunningLock, false);

		if (!FRunning.load(std::memory_order_release)) {
			do {
				Platform::System::Thread::Yield();
			} while (!FRunning.load(std::memory_order_relaxed));
		}
	}
}

CYB::Platform::System::Implementation::Thread::~Thread() {
	DestroyMutex();
}

void* CYB::Platform::System::Implementation::Thread::ThreadProc(void* const AThreadData) {
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

void CYB::Platform::System::Implementation::Thread::DestroyMutex(void) {
	API::Assert::Equal(Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_destroy>(&FRunningLock), 0);
}

bool CYB::Platform::System::Thread::IsFinished(void) const {
	return !FRunning.load(std::memory_order_relaxed);
}

void CYB::Platform::System::Thread::Wait(void) const {
	Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_join>(FThread, nullptr);
}

void CYB::Platform::System::Thread::Sleep(const unsigned int AMilliseconds) {
	Core().FModuleManager.FC.Call<Modules::LibC::usleep>(AMilliseconds * 1000);
}

void CYB::Platform::System::Thread::Yield(void) {
	Core().FModuleManager.FRT.Call<Modules::RT::sched_yield>();
}