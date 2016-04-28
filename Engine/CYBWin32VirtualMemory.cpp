//! @file CYBWin32VirtualMemory.cpp Implements CYB::Platform::VirtualMemory for Win32
#include "CYB.hpp"

void* CYB::Platform::VirtualMemory::Reserve(const unsigned long long ANumBytes) {
	if (ANumBytes >= 1024) {
		void* const Result(Implementation::Win32::VirtualAlloc(nullptr, ANumBytes, MEM_RESERVE, PAGE_NOACCESS));
		if (Result == nullptr)
			throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
		return Result;
	}
	else
		throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
}

void CYB::Platform::VirtualMemory::Commit(void* const AReservation, const unsigned long long ANumBytes) {
	//Const cast is fine since we don't touch the memory
	const void* const Result(Implementation::Win32::VirtualAlloc(AReservation, ANumBytes, MEM_COMMIT, 0));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
}

void CYB::Platform::VirtualMemory::Release(void* const AReservation) {
	if(Implementation::Win32::VirtualFree(AReservation, 0, MEM_RELEASE) == FALSE)
		throw Exception::SystemData(Exception::SystemData::MEMORY_RELEASE_FAILURE);
}

void CYB::Platform::VirtualMemory::Access(void* const AReservation, const AccessLevel AAccessLevel) {
	Implementation::Win32::MEMORY_BASIC_INFORMATION Info;
	if (Implementation::Win32::VirtualQuery(AReservation, &Info, sizeof(Implementation::Win32::MEMORY_BASIC_INFORMATION)) == 0 || Info.State == MEM_FREE)
		throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);

	if (Info.State == MEM_COMMIT) {
		Implementation::Win32::DWORD Last;
		if (Implementation::Win32::VirtualProtect(AReservation, Info.RegionSize, static_cast<Implementation::Win32::DWORD>(AAccessLevel == AccessLevel::READ_WRITE ? PAGE_READWRITE : (AAccessLevel == AccessLevel::NONE ? PAGE_NOACCESS : PAGE_READONLY)), &Last))
			throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);
	}
}

void CYB::Platform::VirtualMemory::Discard(const void* const AMemory, const unsigned long long ANumBytes) {
	static_cast<void>(AMemory);
	static_cast<void>(ANumBytes);
	//! @todo Implement using a K32 optional function set
}