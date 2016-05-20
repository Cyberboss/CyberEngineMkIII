#include "CYB.hpp"

CYB::Platform::Mutex::Mutex() {
	Implementation::Win32::InitializeCriticalSection(&FCriticalSection);
}

CYB::Platform::Mutex::~Mutex() {
	Implementation::Win32::DeleteCriticalSection(&FCriticalSection);
}

void CYB::Platform::Mutex::Lock(void) {
	Implementation::Win32::EnterCriticalSection(&FCriticalSection);
}

bool CYB::Platform::Mutex::TryLock(void) {
	return Implementation::Win32::TryEnterCriticalSection(&FCriticalSection) == TRUE;
}

void CYB::Platform::Mutex::Unlock(void) {
	Implementation::Win32::LeaveCriticalSection(&FCriticalSection);
}