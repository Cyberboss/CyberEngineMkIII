#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::Platform::System::Implementation::Thread::Thread(API::Threadable& AThreadable) :
	FThreadable(AThreadable),
	FRunning(true)
{
	std::atomic_thread_fence(std::memory_order_release);

	if (Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_create>(&FThread, nullptr, ThreadProc, this) != 0) 
		throw Exception::SystemData(Exception::SystemData::THREAD_CREATION_FAILURE);
}

void* CYB::Platform::System::Implementation::Thread::ThreadProc(void* const AThread) noexcept {
	class AutoCleanup {
		std::atomic_bool& FRunning;
	public:
		AutoCleanup(std::atomic_bool& ARunning) noexcept :
			FRunning(ARunning)
		{}
		~AutoCleanup() {
			FRunning.store(false, std::memory_order_release);
		}
	};

	auto& Me(*static_cast<Thread*>(AThread));
	auto& Threadable(Me.FThreadable);
	AutoCleanup Cleaner(Me.FRunning);

	try {
		Threadable.BeginThreadedOperation();
	}
	catch (CYB::Exception::Base AException) {
		//! @todo Log error
		static_cast<void>(AException);
	}
	catch (...) {
		//! @todo Log error
	}

	return nullptr;
}

bool CYB::Platform::System::Thread::IsFinished(void) const noexcept {
	return !FRunning.load(std::memory_order_relaxed);
}

void CYB::Platform::System::Thread::Wait(void) const noexcept {
	Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_join>(FThread, nullptr);
}

void CYB::Platform::System::Thread::Sleep(const unsigned int AMilliseconds) noexcept {
	Core().FModuleManager.FC.Call<Modules::LibC::usleep>(AMilliseconds * 1000);
}