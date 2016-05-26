#include "CYB.hpp"

unsigned int CYB::Platform::Implementation::VirtualMemory::SystemPageSize(void) {
	Win32::SYSTEM_INFO Info;
	Core().FModuleManager.FK32.Call<ModuleDefinitions::KERNEL32::GetSystemInfo>(&Info);
	return Info.dwPageSize;
}

void* CYB::Platform::Implementation::VirtualMemory::PageAlignedUpperBound(void* AMemory, const unsigned int APageSize) {
	return reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(AMemory) + (APageSize - (reinterpret_cast<unsigned long long>(AMemory) % APageSize)));
}

void* CYB::Platform::VirtualMemory::Reserve(const unsigned long long ANumBytes) {
	if (ANumBytes >= 1024) {
		void* const Result(Core().FModuleManager.FK32.Call<ModuleDefinitions::KERNEL32::VirtualAlloc>(nullptr, ANumBytes, Implementation::Win32::DWORD(MEM_RESERVE), Implementation::Win32::DWORD(PAGE_NOACCESS)));
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
		throw;
	}
	const void* const Result(Core().FModuleManager.FK32.Call<ModuleDefinitions::KERNEL32::VirtualAlloc>(AReservation, ANumBytes, Implementation::Win32::DWORD(MEM_COMMIT), Implementation::Win32::DWORD(PAGE_READWRITE)));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
}

void CYB::Platform::VirtualMemory::Release(void* const AReservation) {
	if(Core().FModuleManager.FK32.Call<ModuleDefinitions::KERNEL32::VirtualFree>(AReservation, 0U, Implementation::Win32::DWORD(MEM_RELEASE)) == FALSE)
		throw Exception::SystemData(Exception::SystemData::MEMORY_RELEASE_FAILURE);
}

void CYB::Platform::VirtualMemory::Access(void* const AReservation, const AccessLevel AAccessLevel) {
	Implementation::Win32::MEMORY_BASIC_INFORMATION Info;
	if (Core().FModuleManager.FK32.Call<ModuleDefinitions::KERNEL32::VirtualQuery>(AReservation, &Info, sizeof(Implementation::Win32::MEMORY_BASIC_INFORMATION)) == 0 || Info.State == MEM_FREE)
		throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);

	if (Info.State == MEM_COMMIT) {
		Implementation::Win32::DWORD Last;
		//for some reason we can't use the base of the allocation when protecting :/
		if (Core().FModuleManager.FK32.Call<ModuleDefinitions::KERNEL32::VirtualProtect>(static_cast<unsigned long long*>(AReservation) + 1, Info.RegionSize - 1, static_cast<Implementation::Win32::DWORD>(
			AAccessLevel == AccessLevel::READ_WRITE ? 
			PAGE_READWRITE : 
			(AAccessLevel == AccessLevel::NONE ?
				PAGE_NOACCESS : PAGE_READONLY)),
			&Last))
			throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);
	}
}

void CYB::Platform::VirtualMemory::Discard(void* const AMemory, const unsigned long long ANumBytes) {
	const auto PageSize(Implementation::VirtualMemory::SystemPageSize());
	if (ANumBytes >= PageSize) {
		auto const AlignedMemory(Implementation::VirtualMemory::PageAlignedUpperBound(AMemory, PageSize));
		const auto Difference(reinterpret_cast<unsigned long long>(AlignedMemory) - reinterpret_cast<unsigned long long>(AMemory));
		if (Difference < ANumBytes) {
			const auto BytesAvailableToDiscard(ANumBytes - Difference);
			const auto TrueDiscardSize(BytesAvailableToDiscard - (BytesAvailableToDiscard % PageSize));
			if(TrueDiscardSize >= PageSize
				&& Core().FModuleManager.FK32Extended.Loaded<ModuleDefinitions::Kernel32::DiscardVirtualMemory>())
					Core().FModuleManager.FK32Extended.Call<ModuleDefinitions::Kernel32::DiscardVirtualMemory>(AlignedMemory, TrueDiscardSize);
		}
	}
}