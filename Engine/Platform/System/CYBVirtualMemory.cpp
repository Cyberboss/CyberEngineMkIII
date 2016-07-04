//! @file CYBVirtualMemory.cpp Implements shared functions of CYB::Platform::System::VirtualMemory
#include "CYB.hpp"

CYB::Platform::System::VirtualMemory::VirtualMemory(const unsigned long long ANumBytes) :
	FReservation(InitReservation(ANumBytes)),
	FCommitSize(0),
	FReservationSize(ANumBytes)
{
	API::Assert::LessThanOrEqual(1024ULL, FReservationSize);
}

void* CYB::Platform::System::VirtualMemory::operator()(void) const noexcept {
	return FReservation;
}

unsigned long long CYB::Platform::System::VirtualMemory::CommitSize(void) const noexcept {
	return FCommitSize;
}

unsigned long long CYB::Platform::System::VirtualMemory::ReservationSize(void) const noexcept {
	return FReservationSize;
}