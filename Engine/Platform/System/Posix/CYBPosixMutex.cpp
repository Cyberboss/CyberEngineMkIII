#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::Platform::System::Implementation::Mutex::Mutex() :
	FMutex(PTHREAD_MUTEX_INITIALIZER)
{}

CYB::Platform::System::Mutex::Mutex() {
	if (Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_init>(&FMutex, nullptr) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
}

CYB::Platform::System::Mutex::~Mutex() {
	const auto Result(Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_destroy>(&FMutex));
	API::Assert::Equal(Result, 0);
}

void CYB::Platform::System::Mutex::Lock(void) const noexcept {
	Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_lock>(&FMutex);
}

bool CYB::Platform::System::Mutex::TryLock(void) const noexcept {
	return Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_trylock>(&FMutex) == 0;
}

void CYB::Platform::System::Mutex::Unlock(void) const noexcept {
	Core().FModuleManager.FPThread.Call<Modules::PThread::pthread_mutex_unlock>(&FMutex);
}