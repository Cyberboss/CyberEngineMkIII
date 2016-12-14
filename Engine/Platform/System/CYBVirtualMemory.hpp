//! @file CYBVirtualMemory.hpp Contains the platform memory manager functions
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Access and abstraction to the basic OS memory functions
			class VirtualMemory : private Implementation::VirtualMemory {
			public:
				//! @brief Memory permissions
				enum class AccessLevel {
					READ,	//!< @brief Memory can be read
					READ_WRITE,	//!< @brief Memory can be read and written to
					NONE,	//!< @brief Memory cannot be accessed
				};
			private:
				void* const FReservation;	//!< @brief The pointer to the reservation
				unsigned long long FCommitSize;	//!< @brief The current size of commited memory in the reservation
				const unsigned long long FReservationSize;	//!< @brief The size of the reservation
			private:
				/*!
					@brief Reserve some amount of address space for future allocation
					@param ANumBytes The number of bytes in the address space to reserve. Must be at least 1024. Will be rounded up to the nearest system page
					@return A pointer to a reservation of size @p ANumBytes
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MEMORY_RESERVATION_FAILURE. Thrown if the reservation was unable to be made
				*/
				static void* InitReservation(const unsigned long long ANumBytes);
			public:
				/*!
					@brief Reserve some amount of address space for future allocation
					@param ANumBytes The number of bytes in the address space to reserve. Must be at least 1024. Will be rounded up to the nearest system page
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MEMORY_RESERVATION_FAILURE. Thrown if the reservation was unable to be made
				*/
				VirtualMemory(const unsigned long long ANumBytes);
				//! @brief Release a reservation. All memory that was used in entire reserved address space will be lost
				~VirtualMemory();

				/*!
					@brief Commit some amount of memory from a reserved address space. On return, the memory will be usable. The access of the reservation will be AccessLevel::READ_WRITE upon return if it succeeds, or the previous AccessLevel otherwise
					@param ANumBytes The number of bytes in the address space to commit, will be rounded up to the nearest reservable granularity
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MEMORY_COMMITAL_FAILURE. Thrown if the commital was unable to be made
				*/
				void Commit(const unsigned long long ANumBytes);
				/*!
					@brief Designate an area of memory as not needed for the time being, improving performance. It may be reaccessed at any time, however, it should be assumed to be filled with random data upon doing so
					@param AMemory An address in a committed area of memory
					@param ANumBytes The number of bytes to discard, will be rounded down to the nearest discardable granularity. Must be less than reservation size
					@par Thread Safety
						This function requires no thread safety, though it is not atomic
				*/
				void Discard(void* const AMemory, const unsigned long long ANumBytes) noexcept;
				/*!
					@brief Set the memory protection on a reservation. Does nothing without a previous commit
					@param AAccess The access level to set
					@par Thread Safety
						This function requires no thread safety, though it is not atomic
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MEMORY_PROTECT_FAILURE. Thrown if the commital was unable to be made
				*/
				void Access(const AccessLevel AAccess);
				/*!
					@brief Get the underlying reservation
					@return The memory region for the memory mapping
					@par Thread Safety
						This function requires no thread safety
				*/
				void* operator()(void) const noexcept;
				/*!
					@brief Get the current commit size of the reservation
					@return The current commit size of the reservation
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				unsigned long long CommitSize(void) const noexcept;
				/*!
					@brief Get the reservation size. Attempting to commit a size larger than this will generate a throw
					@return The current size of the reservation
					@par Thread Safety
						This function requires no thread safety
				*/
				unsigned long long ReservationSize(void) const noexcept;
			};
		};
	};
};