#include "TestHeader.hpp"

SCENARIO("VirtualMemory reservations handle various sizes", "[VirtualMemory]") {
	GIVEN("Some reservation size") {
		unsigned long long ReservationSize;
		WHEN("The size is sane") {
			ReservationSize = 1000000;
			THEN("The range is reserved and result is not null") {
				REQUIRE(CYB::Platform::VirtualMemory::Reserve(ReservationSize) != nullptr);
			}
		}
		WHEN("The size is insanely small") {
			ReservationSize = 12;
			bool Error(false);
			void* Result(nullptr);
			try {
				Result = CYB::Platform::VirtualMemory::Reserve(ReservationSize);
			}
			catch (CYB::Exception::SystemData AException) {
				Error = AException.FErrorCode == CYB::Exception::SystemData::MEMORY_RESERVATION_FAILURE;
			}
			THEN("The apporpriate error is thrown") {
				REQUIRE(Error);
				REQUIRE(Result == nullptr);
			}
		}
		WHEN("The size is insanely large") {
			ReservationSize = std::numeric_limits<unsigned long long>().max();
			bool Error(false);
			void* Result(nullptr);
			try {
				Result = CYB::Platform::VirtualMemory::Reserve(ReservationSize);
			}
			catch (CYB::Exception::SystemData AException) {
				Error = AException.FErrorCode == CYB::Exception::SystemData::MEMORY_RESERVATION_FAILURE;
			}
			THEN("The apporpriate error is thrown") {
				REQUIRE(Error);
				REQUIRE(Result == nullptr);
			}
		}
	}
}