#include "CYB.hpp"

using namespace CYB::Platform::Implementation::Posix;

CYB::Platform::Implementation::Mutex::Mutex() :
	FMutex(PTHREAD_MUTEX_INITIALIZER)
{}

CYB::Platform::Mutex::Mutex() {
	if (Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_init>(&FMutex, nullptr) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE);
}

CYB::Platform::Mutex::~Mutex() {
	if(Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_destroy>(&FMutex) != 0)
		throw CYB::Exception::SystemData(CYB::Exception::SystemData::MUTEX_DESTRUCTION_FAILURE);
}

void CYB::Platform::Mutex::Lock(void) const {
	Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_lock>(&FMutex);
}

bool CYB::Platform::Mutex::TryLock(void) const {
	return Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_trylock>(&FMutex) == 0;
}

void CYB::Platform::Mutex::Unlock(void) const {
	Core().FModuleManager.FPThread.Call<ModuleDefinitions::PThread::pthread_mutex_unlock>(&FMutex);
}