//! @file CYBFile.cpp Implements CYB::Platform::System::Semaphore logic and alogorithms
#include "CYB.hpp"

void CYB::Platform::System::Semaphore::SignalAll(void) noexcept {
	Lock();
	FWakeCount += FSleepCount;
	WakeAll();
	Unlock();
}

void CYB::Platform::System::Semaphore::SignalN(const unsigned long long AN) noexcept {
	FWakeCount += AN;
	for (auto I(0ULL); I < AN; ++I)
		WakeOne();
}

void CYB::Platform::System::Semaphore::Wait(void) noexcept {
	Lock();

	const auto WaitingFor(FServiceCount);
	++FServiceCount;

	API::Assert::NotEqual(FServiceCount, 0ULL);

	bool NotFirst(false);
	while (FWakeCount < FServiceCount) {
		if (NotFirst)
			WakeOne();

		++FSleepCount;
		std::atomic_thread_fence(std::memory_order_release);
		EnterCV();
		std::atomic_thread_fence(std::memory_order_acquire);
		--FSleepCount;

		NotFirst = true;
	}

	if (FSleepCount == 0) {
		//take this opportunity to reset the counters
		//overflow will never happen in a million years, but it would be BAD
		FServiceCount = 0;
		FWakeCount = 0;
		std::atomic_thread_fence(std::memory_order_release);
	}

	Unlock();
}