//! @file CYBWin32VirtualMemory.cpp Implements CYB::Platform::VirtualMemory for Win32
#include "CYB.hpp"

void* CYB::Platform::VirtualMemory::Reserve(const unsigned long long ANumBytes) {
	if (ANumBytes >= 1024) {
		void* const Result(Implementation::Win32::VirtualAlloc(nullptr, ANumBytes, MEM_RESERVE, PAGE_NOACCESS));
		if (Result != nullptr)
			return Result;
	}
	throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
}

void CYB::Platform::VirtualMemory::Commit(void* const AReservation, const unsigned long long ANumBytes) {
	try {
		Access(AReservation, AccessLevel::READ_WRITE);
	}
	catch (Exception::SystemData AException) {
		if(AException.FErrorCode == Exception::SystemData::MEMORY_PROTECT_FAILURE)
			throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
		throw AException;
	}
	const void* const Result(Implementation::Win32::VirtualAlloc(AReservation, ANumBytes, MEM_COMMIT, PAGE_READWRITE));
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
		//for some reason we can't use the base of the allocation when protecting :/
		if (Implementation::Win32::VirtualProtect(static_cast<unsigned long long*>(AReservation) + 1, Info.RegionSize - 1, static_cast<Implementation::Win32::DWORD>(AAccessLevel == AccessLevel::READ_WRITE ? PAGE_READWRITE : (AAccessLevel == AccessLevel::NONE ? PAGE_NOACCESS : PAGE_READONLY)), &Last)) 
			throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);
	}
}

void CYB::Platform::VirtualMemory::Discard(const void* const AMemory, const unsigned long long ANumBytes) {
	static_cast<void>(AMemory);
	static_cast<void>(ANumBytes);
	//! @todo Implement using a K32 optional function set
}