//! @file CYBFile.cpp Implements CYB::Platform::System::Semaphore shared functions
#include "CYB.hpp"

void CYB::Platform::System::Semaphore::SignalAll(void) noexcept {
	UNIMPLEMENTED;
}

void CYB::Platform::System::Semaphore::SignalN(const unsigned long long AN) noexcept {
	static_cast<void>(AN);
	UNIMPLEMENTED;
}

void CYB::Platform::System::Semaphore::Wait() const noexcept {
	UNIMPLEMENTED;
}

bool CYB::Platform::System::Semaphore::WaitN(const unsigned int AMilliseconds) const noexcept {
	static_cast<void>(AMilliseconds);
	UNIMPLEMENTED;
	return false;
}