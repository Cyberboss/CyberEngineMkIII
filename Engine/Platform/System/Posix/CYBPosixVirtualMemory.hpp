#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class VirtualMemory {
				protected:
					enum {
						SUPERBLOCK_SIZE = 2 * sizeof(unsigned long long),
					};
				protected:
					static bool AccessSuperblock(void* const AReservation) noexcept;

					static unsigned long long* GetReservationFromSuperblock(void* const ASuperblock) noexcept;
					static unsigned long long* GetSuperblockFromReservation(void* const AReservation) noexcept;
				};
			};
		};
	};
};