//! @file CYBPosixVirtualMemory.cpp Implements CYB::Platform::VirtualMemory under Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

void* CYB::Platform::System::VirtualMemory::InitReservation(const unsigned long long ANumBytes) {
	API::Assert::LessThanOrEqual(1024ULL, ANumBytes);
	auto const Reservation(reinterpret_cast<unsigned long long*>(Core().FModuleManager.FC.Call<Modules::LibC::mmap>(nullptr, ANumBytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)));

	if (Reservation == MAP_FAILED) 
		throw Exception::Internal(Exception::Internal::MEMORY_RESERVATION_FAILURE);
	return Reservation;
}

CYB::Platform::System::VirtualMemory::~VirtualMemory() {
	API::Assert::Equal(Core().FModuleManager.FC.Call<Modules::LibC::munmap>(FReservation, FReservationSize), 0);
}

void CYB::Platform::System::VirtualMemory::Commit(const unsigned long long ANumBytes) {
	if (ANumBytes > FCommitSize) {
		if(Core().FModuleManager.FC.Call<Modules::LibC::mprotect>(FReservation, ANumBytes, PROT_READ | PROT_WRITE) != 0)
			throw Exception::Internal(Exception::Internal::MEMORY_COMMITAL_FAILURE);
		FCommitSize = ANumBytes;
	}
}

void CYB::Platform::System::VirtualMemory::Access(const AccessLevel AAccessLevel) {
	if (Core().FModuleManager.FC.Call<Modules::LibC::mprotect>(FReservation, FCommitSize, AAccessLevel == AccessLevel::READ_WRITE ? PROT_READ | PROT_WRITE : (AAccessLevel == AccessLevel::READ ? PROT_READ : PROT_NONE)) != 0)
		throw Exception::Internal(Exception::Internal::MEMORY_PROTECT_FAILURE);
}

void CYB::Platform::System::VirtualMemory::Discard(void* const AMemory, const unsigned long long ANumBytes) noexcept {
	API::Assert::LessThanOrEqual(static_cast<byte*>(FReservation), static_cast<byte*>(AMemory));
	API::Assert::LessThanOrEqual(static_cast<byte*>(AMemory), static_cast<byte*>(FReservation) + FCommitSize);
	API::Assert::LessThanOrEqual(static_cast<byte*>(AMemory) + ANumBytes, static_cast<byte*>(FReservation) + FCommitSize);
	Core().FModuleManager.FC.Call<Modules::LibC::madvise>(AMemory, ANumBytes, MADV_DONTNEED);
}