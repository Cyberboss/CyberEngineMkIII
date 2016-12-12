//! @file CYBPosixSemaphore.cpp Implements CYB::Platform::System::Semaphore for Win32
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

CYB::Platform::System::Implementation::Semaphore::Semaphore() :
	FMutex(PTHREAD_MUTEX_INITIALIZER),
	FCondVar(PTHREAD_COND_INITIALIZER)
{}

CYB::Platform::System::Semaphore::Semaphore() {
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