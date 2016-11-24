#include "CYB.hpp"

unsigned int CYB::Platform::System::Implementation::VirtualMemory::SystemPageSize(void) noexcept {
	Win32::SYSTEM_INFO Info;
	Core().FModuleManager.FK32.Call<Modules::Kernel32::GetSystemInfo>(&Info);
	return Info.dwPageSize;
}

void* CYB::Platform::System::Implementation::VirtualMemory::PageAlignedUpperBound(void* AMemory, const unsigned int APageSize) noexcept {
	return reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(AMemory) + (APageSize - (reinterpret_cast<unsigned long long>(AMemory) % APageSize)));
}

void* CYB::Platform::System::VirtualMemory::InitReservation(const unsigned long long ANumBytes) {
	if (ANumBytes >= 1024) {
		void* const Result(Core().FModuleManager.FK32.Call<Modules::Kernel32::VirtualAlloc>(nullptr, ANumBytes, Win32::DWORD(MEM_RESERVE), Win32::DWORD(PAGE_NOACCESS)));
		if (Result != nullptr)
			return Result;
	}
	throw Exception::Internal(Exception::Internal::MEMORY_RESERVATION_FAILURE);
}

CYB::Platform::System::VirtualMemory::~VirtualMemory() {
	API::Assert::NotEqual<Win32::BOOL>(Core().FModuleManager.FK32.Call<Modules::Kernel32::VirtualFree>(FReservation, 0U, Win32::DWORD(MEM_RELEASE)), FALSE);
}

void CYB::Platform::System::VirtualMemory::Commit(const unsigned long long ANumBytes) {
	if (ANumBytes > FCommitSize) {
		if (ANumBytes <= FReservationSize) {
			bool ThrowOccurred(false);
			try {
				Access(AccessLevel::READ_WRITE);
			}
			catch (Exception::Internal AException) {
				API::Assert::Equal<unsigned int>(AException.FErrorCode, Exception::Internal::MEMORY_PROTECT_FAILURE);
				ThrowOccurred = true;
			}
			if (ThrowOccurred)
				throw Exception::Internal(Exception::Internal::MEMORY_COMMITAL_FAILURE);
			const void* const Result(Core().FModuleManager.FK32.Call<Modules::Kernel32::VirtualAlloc>(FReservation, ANumBytes, Win32::DWORD(MEM_COMMIT), Win32::DWORD(PAGE_READWRITE)));
			if (Result == nullptr)
				throw Exception::Internal(Exception::Internal::MEMORY_COMMITAL_FAILURE);
			FCommitSize = ANumBytes;
		}
		else
			throw Exception::Internal(Exception::Internal::MEMORY_COMMITAL_FAILURE);
	}
}

void CYB::Platform::System::VirtualMemory::Access(const AccessLevel AAccessLevel) {
	Win32::MEMORY_BASIC_INFORMATION Info;
	if (Core().FModuleManager.FK32.Call<Modules::Kernel32::VirtualQuery>(FReservation, &Info, sizeof(Win32::MEMORY_BASIC_INFORMATION)) == 0)
		throw Exception::Internal(Exception::Internal::MEMORY_PROTECT_FAILURE);

	if (Info.State == MEM_COMMIT) {
		Win32::DWORD Last;
		//for some reason we can't use the base of the allocation when protecting :/
		if (Core().FModuleManager.FK32.Call<Modules::Kernel32::VirtualProtect>(FReservation, Info.RegionSize, static_cast<Win32::DWORD>(
			AAccessLevel == AccessLevel::READ_WRITE ? 
			PAGE_READWRITE : 
			(AAccessLevel == AccessLevel::NONE ?
				PAGE_NOACCESS : PAGE_READONLY)),
			&Last) == FALSE)
			throw Exception::Internal(Exception::Internal::MEMORY_PROTECT_FAILURE);
	}
}

void CYB::Platform::System::VirtualMemory::Discard(void* const AMemory, const unsigned long long ANumBytes) noexcept {
	API::Assert::LessThanOrEqual(static_cast<byte*>(FReservation), static_cast<byte*>(AMemory));
	API::Assert::LessThanOrEqual(static_cast<byte*>(AMemory), static_cast<byte*>(FReservation) + FCommitSize);
	API::Assert::LessThanOrEqual(static_cast<byte*>(AMemory) + ANumBytes, static_cast<byte*>(FReservation) + FCommitSize);
	const auto PageSize(Implementation::VirtualMemory::SystemPageSize());
	if (ANumBytes >= PageSize) {
		auto const AlignedMemory(Implementation::VirtualMemory::PageAlignedUpperBound(AMemory, PageSize));
		const auto Difference(reinterpret_cast<unsigned long long>(AlignedMemory) - reinterpret_cast<unsigned long long>(AMemory));
		if (Difference < ANumBytes) {
			const auto BytesAvailableToDiscard(ANumBytes - Difference);
			const auto TrueDiscardSize(BytesAvailableToDiscard - (BytesAvailableToDiscard % PageSize));
			if(TrueDiscardSize >= PageSize)
				if(Core().FModuleManager.FK32Extended.Loaded<Modules::Kernel32Extended::DiscardVirtualMemory>())
					Core().FModuleManager.FK32Extended.Call<Modules::Kernel32Extended::DiscardVirtualMemory>(AlignedMemory, TrueDiscardSize);
		}
	}
}