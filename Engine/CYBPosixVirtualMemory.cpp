//! @file CYBPosixVirtualMemory.cpp Implements CYB::Platform::VirtualMemory for Posix
#include "CYB.hpp"

void* CYB::Platform::VirtualMemory::Reserve(unsigned long long ANumBytes) {
	if (ANumBytes >= 1024) {
		using namespace Implementation::Posix;
		const auto SuperblockSize(2 * sizeof(unsigned long long));
		ANumBytes += SuperblockSize;
		auto Result(reinterpret_cast<unsigned long long*>(Sys::Call(Sys::MMAP, nullptr, ANumBytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)));
		//when coming from the kernel, error values are negative >:(
		if (Result != nullptr && reinterpret_cast<unsigned long long>(Result) > static_cast<unsigned long long>(-4096))
			throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);

		//we still have to store the size and committal in the actual allocation because mmap thinks it's special

		if (Sys::Call(Sys::MPROTECT, Result, SuperblockSize, PROT_READ | PROT_WRITE) == 0) {
			Result += 2;
			Result[-2] = ANumBytes;
			Result[-1] = SuperblockSize;
			try {
				Access(Result, AccessLevel::NONE);
			}
			catch (Exception::SystemData AException) {
				//this really should never happen, gotta be safe though
				if (AException.FErrorCode == Exception::SystemData::MEMORY_PROTECT_FAILURE)
					try {
					Release(Result);
					//if this fails, well now we have a useless reservation shitting all over our address space and there's nothing we can do about it
				}
				catch (Exception::SystemData ADoubleFault) {
					throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
				}
				else if (AException.FErrorCode == Exception::SystemData::MEMORY_COMMITAL_FAILURE)
					throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
				else
					throw AException;
			}
		}
		else {
			Sys::Call(Sys::MUNMAP, Result - 2, ANumBytes);
			throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
		}

		return Result;
	}else
		throw Exception::SystemData(Exception::SystemData::MEMORY_RESERVATION_FAILURE);
}

void CYB::Platform::VirtualMemory::Commit(void* const AReservation, unsigned long long ANumBytes) {
	using namespace Implementation::Posix;
	auto Base(static_cast<unsigned long long*>(AReservation) - 2);
	ANumBytes += 2 * sizeof(unsigned long long);
	if (Sys::Call(Sys::MPROTECT, Base, 2 * sizeof(unsigned long long), PROT_READ | PROT_WRITE) == 0) {
		if (ANumBytes > Base[1]) {
			if (ANumBytes <= Base[0]) {
				if (Sys::Call(Sys::MPROTECT, Base, ANumBytes, PROT_READ | PROT_WRITE) == 0)
					Base[1] = ANumBytes;
				else
					throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
			}
			else
				throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
		}
	}
	else
		throw Exception::SystemData(Exception::SystemData::MEMORY_COMMITAL_FAILURE);
}

void CYB::Platform::VirtualMemory::Release(void* const AReservation) {
	try {
		Access(AReservation, AccessLevel::READ);
	}catch(Exception::SystemData AException){
		if(AException.FErrorCode == Exception::SystemData::MEMORY_PROTECT_FAILURE)
			throw Exception::SystemData(Exception::SystemData::MEMORY_RELEASE_FAILURE);
		throw AException;
	}
	auto Base(static_cast<unsigned long long*>(AReservation) - 2);
	if(Sys::Call(Sys::MUNMAP, Base, *Base) != 0)
		throw Exception::SystemData(Exception::SystemData::MEMORY_RELEASE_FAILURE);
}

void CYB::Platform::VirtualMemory::Access(void* const AReservation, const AccessLevel AAccessLevel) {
	using namespace Implementation::Posix;
	auto Base(static_cast<unsigned long long*>(AReservation) - 2);
	if (Sys::Call(Sys::MPROTECT, Base, Base[1], AAccessLevel == AccessLevel::READ_WRITE ? PROT_READ | PROT_WRITE : (AAccessLevel == AccessLevel::READ ? PROT_READ : PROT_NONE)) != 0)
		throw Exception::SystemData(Exception::SystemData::MEMORY_PROTECT_FAILURE);
}

void CYB::Platform::VirtualMemory::Discard(const void* const AMemory, const unsigned long long ANumBytes) {
	static_cast<void>(AMemory);
	static_cast<void>(ANumBytes);
	//! @todo Implement using a K32 optional function set
}