//! @file CYBPosixSemaphore.cpp Implements CYB::Platform::System::Semaphore for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::Platform::System::Implementation::Semaphore::Semaphore() noexcept :
	FMutex(PTHREAD_MUTEX_INITIALIZER),
	FCondVar(PTHREAD_COND_INITIALIZER)
{}

CYB::Platform::System::Semaphore::Semaphore() :
	FServiceCount(1),
	FWakeCount(0),
	FSleepCount(0)
{
	if (Core().FModuleManager.Call<Modules::PThread::pthread_mutex_init>(&FMutex, nullptr) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
	if (Core().FModuleManager.Call<Modules::PThread::pthread_cond_init>(&FCondVar, nullptr) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
}

CYB::Platform::System::Semaphore::~Semaphore() {
	const auto Result1(Core().FModuleManager.Call<Modules::PThread::pthread_cond_destroy>(&FCondVar));
	API::Assert::Equal(Result1, 0);
	const auto Result2(Core().FModuleManager.Call<Modules::PThread::pthread_mutex_destroy>(&FMutex));
	API::Assert::Equal(Result2, 0);
}

void CYB::Platform::System::Semaphore::Lock(void) noexcept {
	Core().FModuleManager.Call<Modules::PThread::pthread_mutex_lock>(&FMutex);
}

void CYB::Platform::System::Semaphore::Unlock(void) noexcept {
	Core().FModuleManager.Call<Modules::PThread::pthread_mutex_unlock>(&FMutex);
}

void CYB::Platform::System::Semaphore::EnterCV(void) noexcept {
	Core().FModuleManager.Call<Modules::PThread::pthread_cond_wait>(&FCondVar, &FMutex);
}

//POSIX is weird in that it needs the lock in order to signal
//Be aware

void CYB::Platform::System::Implementation::Semaphore::WakeAll(void) noexcept {
	Core().FModuleManager.Call<Modules::PThread::pthread_cond_broadcast>(&FCondVar);
}

void CYB::Platform::System::Implementation::Semaphore::WakeOne(void) noexcept {
	Core().FModuleManager.Call<Modules::PThread::pthread_cond_signal>(&FCondVar);
}