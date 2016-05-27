#pragma once

inline CYB::API::LockGuard::LockGuard(Mutex& AMutex) :
	FMutex(&AMutex)
{
	FMutex->Lock();
}

inline CYB::API::LockGuard::LockGuard(LockGuard&& AMove) :
	FMutex(AMove.FMutex)
{
	AMove.FMutex = nullptr;
}

inline CYB::API::LockGuard& CYB::API::LockGuard::operator=(LockGuard&& AMove) {
	FMutex = AMove.FMutex;
	AMove.FMutex = nullptr;
	return *this;
}

inline CYB::API::LockGuard::~LockGuard() {
	Release();
}

inline void CYB::API::LockGuard::Release(void) {
	if (FMutex != nullptr) {
		FMutex->Unlock();
		FMutex = nullptr;
	}
}