#include "CYB.hpp"

//Superblock
//0 - 7: Reservation size
//8 - 15: Commit size

bool CYB::Platform::System::Implementation::VirtualMemory::AccessSuperblock(void* const AReservation) noexcept {
	return Core().FModuleManager.FC.Call<Modules::LibC::mprotect>(GetSuperblockFromReservation(AReservation), SUPERBLOCK_SIZE, PROT_READ | PROT_WRITE) == 0;
}

unsigned long long* CYB::Platform::System::Implementation::VirtualMemory::GetSuperblockFromReservation(void* const AReservation) noexcept {
	static_assert(SUPERBLOCK_SIZE % sizeof(unsigned long long) == 0, "The Posix VM superblock has changed and the access methods must as well");
	return static_cast<unsigned long long*>(AReservation) - (SUPERBLOCK_SIZE / sizeof(unsigned long long));
}
unsigned long long* CYB::Platform::System::Implementation::VirtualMemory::GetReservationFromSuperblock(void* const ASuperblock) noexcept {
	return static_cast<unsigned long long*>(ASuperblock) + (SUPERBLOCK_SIZE / sizeof(unsigned long long));
}

void* CYB::Platform::System::VirtualMemory::Reserve(unsigned long long ANumBytes) {
	if (ANumBytes >= 1024) {
		using namespace Posix;
		ANumBytes += SUPERBLOCK_SIZE;
		auto const Superblock(reinterpret_cast<unsigned long long*>(Core().FModuleManager.FC.Call<Modules::LibC::mmap>(nullptr, ANumBytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)));
		//when coming from the kernel, error values are negative >:(
		if (reinterpret_cast<unsigned long long>(Superblock) != -1ULL) {

			//we still have to store the size and committal in the actual allocation because mmap thinks it's special

			auto const Reservation(GetReservationFromSuperblock(Superblock));
			if (AccessSuperblock(Reservation)) {
				Superblock[0] = ANumBytes;
				Superblock[1] = SUPERBLOCK_SIZE;
				try {
					Access(Reservation, AccessLevel::NONE);
				}
				catch (Exception::SystemData AException) {
					//this really should never happen, gotta be safe though
					if (AException.FErrorCode == Exception::SystemData::MEMORY_PROTECT_FAILURE) {
						try {
							Release(Reservation);
							//if this fails, well now we have a useless reservation shitting all over our address space and there's nothing we can do about it
						}
						catch (Exception::SystemData ADoubleFault) {
							static_cast<void>(ADoubleFault);
							throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
						}
					}
					else {
						API::Assert::Equal(AException.FErrorCode, static_cast<unsigned int>(Exception::SystemData::MEMORY_COMMITAL_FAILURE));
						throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
					}
				}
			}
			else {
				Core().FModuleManager.FC.Call<Modules::LibC::munmap>(Superblock, ANumBytes);
				throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
			}

			return Reservation;
		}
	}
	throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);}	//gcov bitching

void CYB::Platform::System::VirtualMemory::Commit(void* const AReservation, unsigned long long ANumBytes) {
	using namespace Posix;
	auto const Superblock(GetSuperblockFromReservation(AReservation));
	ANumBytes += SUPERBLOCK_SIZE;
	if (AccessSuperblock(AReservation)) {
		if (ANumBytes > Superblock[1]) {
			if (ANumBytes <= Superblock[0] && Core().FModuleManager.FC.Call<Modules::LibC::mprotect>(Superblock, ANumBytes, PROT_READ | PROT_WRITE) == 0)
				Superblock[1] = ANumBytes;
			else
				throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
		}
	}
	else
		throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
}

void CYB::Platform::System::VirtualMemory::Release(void* const AReservation) {
	auto const Superblock(GetSuperblockFromReservation(AReservation));
	if(!AccessSuperblock(AReservation) || Core().FModuleManager.FC.Call<Modules::LibC::munmap>(Superblock, *Superblock) != 0)
		throw Exception::SystemData(Exception::SystemData::MEMORY_RELEASE_FAILURE);
}

void CYB::Platform::System::VirtualMemory::Access(void* const AReservation, const AccessLevel AAccessLevel) {
	using namespace Posix;
	auto const Superblock(GetSuperblockFromReservation(AReservation));
	if (!AccessSuperblock(AReservation) || Core().FModuleManager.FC.Call<Modules::LibC::mprotect>(Superblock, Superblock[1], AAccessLevel == AccessLevel::READ_WRITE ? PROT_READ | PROT_WRITE : (AAccessLevel == AccessLevel::READ ? PROT_READ : PROT_NONE)) != 0)
		throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);
}

void CYB::Platform::System::VirtualMemory::Discard(void* const AMemory, const unsigned long long ANumBytes) noexcept {
	using namespace Posix;
	Core().FModuleManager.FC.Call<Modules::LibC::madvise>(AMemory, ANumBytes, MADV_DONTNEED);
}