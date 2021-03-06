#pragma once

inline CYB::API::LockGuard::LockGuard(Mutex& AMutex, const bool APreLocked) noexcept :
	FMutex(&AMutex)
{
	if(!APreLocked)
		FMutex->Lock();
}

inline CYB::API::LockGuard::LockGuard(LockGuard&& AMove) noexcept :
	FMutex(AMove.FMutex)
{
	AMove.FMutex = nullptr;
}

inline CYB::API::LockGuard& CYB::API::LockGuard::operator=(LockGuard&& AMove) noexcept {
	FMutex = AMove.FMutex;
	AMove.FMutex = nullptr;
	return *this;
}

inline CYB::API::LockGuard::~LockGuard() {
	Release();
}

inline void CYB::API::LockGuard::Release(void) noexcept {
	if (FMutex != nullptr) {
		FMutex->Unlock();
		FMutex = nullptr;
	}
}