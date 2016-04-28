//! @file CYBThread.hpp Contains the platform memory manager functions
#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Access and abstraction to the basic OS memory functions
		class VirtualMemory {
		public:
			enum class AccessLevel {
				READ,
				READ_WRITE,
				NONE,
			};
		public:
			/*!
				@brief Reserve some amount of address space for future allocation
				@param ANumBytes The number of bytes in the address space to reserve, will be rounded up to the nearest system page
				@return The reserved address space
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_RESERVATION_FAILURE if the reservation was unable to be made
			*/
			static void* Reserve(unsigned long long ANumBytes);
			/*!
				@brief Commit some amount of memory from a reserved address space. On return, the memory will be usable. The access of the reservation will be AccessLevel::READ_WRITE upon return
				@param AReservation An address returned from Reserve
				@param ANumBytes The number of bytes in the address space to commit, will be rounded up to the nearest reservable granularity
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE if the commital was unable to be made
			*/
			static void Commit(void* const AReservation, unsigned long long ANumBytes);
			/*!
				@brief Designate an area of memory as not needed for the time being, improving performance. It may be reaccessed at any time, however, it should be assumed to be filled with random data upon doing so
				@param AMemory An address in a committed area of memory
				@param ANumBytes The number of bytes to discard, will be rounded down to the nearest discardable granularity. Must be less than reservation size
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_DISCARD_FAILURE if the commital was unable to be made
			*/
			static void Discard(const void* const AMemory, const unsigned long long ANumBytes);
			/*!
				@brief Undo a reservation, improving performance. All memory that was used in entire reserved address space will be lost
				@param AReservation An address returned from ReserveMemory
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_RELEASE_FAILURE if the commital was unable to be made
			*/
			static void Release(void* AReservation);
			/*!
				@brief Set the memory protection on a reservation
				@param AReservation An address returned from Reserve
				@param AAccess The access level of the 
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MEMORY_PROTECT_FAILURE if the commital was unable to be made
			*/
			static void Access(void* AReservation, const AccessLevel AAccess);
		};
	};
};