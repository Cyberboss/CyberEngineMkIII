#include "TestHeader.hpp"
SCENARIO("VirtualMemory reservations can be made and released", "[VirtualMemory]") {
	GIVEN("A sane reservation size") {
		const auto ReservationSize(1000000);
		WHEN("A reservation is made") {
			bool Error(false);
			void* Result(nullptr);
			try {
				Result = CYB::Platform::VirtualMemory::Reserve(ReservationSize);
			}
			catch (...) {
				Error = true;
			}
			THEN("No errors occur and a valid address is returned") {
				REQUIRE(Result != nullptr);
				REQUIRE(!Error);
			}
		}
	}
	GIVEN("An existing reservation of sane size") {
		void* const Result(CYB::Platform::VirtualMemory::Reserve(1000000));
		WHEN("The reservation is released") {
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Release(Result);
			}
			catch (...) {
				Error = true;
			}
			THEN("No errors occur") {
				REQUIRE(!Error);
			}
		}
	}
}
SCENARIO("VirtualMemory reservations handle various sizes", "[VirtualMemory]") {
	GIVEN("Some reservation size") {
		unsigned long long ReservationSize;
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
#ifndef TARGET_OS_LINUX
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
#endif
	}
}
SCENARIO("VirtualMemory reservations handle various commit sizes", "[VirtualMemory]") {
	GIVEN("A standard reservation") {
		auto Reservation(CYB::Platform::VirtualMemory::Reserve(1000000));
		unsigned long long CommitSize;
		WHEN("The size is normal") {
			CommitSize = 4096;
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Commit(Reservation, CommitSize);
			}
			catch (...) {
				Error = true;
			}
			THEN("The commit works as normal") {
				REQUIRE(!Error);
			}
		}
		WHEN("The size is insanely small") {
			CommitSize = 1;
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Commit(Reservation, CommitSize);
			}
			catch (...) {
				Error = true;
			}
			THEN("The commit works as normal, but nothing changes") {
				REQUIRE(!Error);
			}
		}
		WHEN("The size is larger than the reservation") {
			CommitSize = 2000000;
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Commit(Reservation, CommitSize);
			}
			catch (CYB::Exception::SystemData AException) {
				Error = AException.FErrorCode == CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE;
			}
			THEN("The commit fails") {
				REQUIRE(Error);
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}

	GIVEN("A standard reservation and commit") {
		auto Reservation(CYB::Platform::VirtualMemory::Reserve(1000000));
		CYB::Platform::VirtualMemory::Commit(Reservation, 500000);
		WHEN("A smaller commit is made") {
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Commit(Reservation, 1000);
			}
			catch (...) {
				Error = true;
			}
			THEN("No changes are made but the function succeeds") {
				REQUIRE(!Error);
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}
}
SCENARIO("VirtualMemory reservation protection levels can be changed", "[VirtualMemory]") {
	GIVEN("A standard reservation and commit which has some data written to it") {
		auto Reservation(static_cast<unsigned long long*>(CYB::Platform::VirtualMemory::Reserve(1000000)));
		CYB::Platform::VirtualMemory::Commit(Reservation, 500000);
		*Reservation = 1234;
		WHEN("The access level is set to NONE") {
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::NONE);
			}
			catch (...) {
				Error = true;
			}
			THEN("No errors occur and pages cannot be used") {
				REQUIRE(!Error);
			}
		}
		WHEN("The access level is set to READ") {
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::READ);
			}
			catch (...) {
				Error = true;
			}
			THEN("No errors occur and pages can be read") {
				REQUIRE(!Error);
				REQUIRE(*Reservation == 1234U);
			}
		}
		WHEN("The access level is set to READ_WRITE") {
			bool Error(false);
			try {
				CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::READ_WRITE);
			}
			catch (...) {
				Error = true;
			}
			THEN("No errors occur and pages can be read and written") {
				REQUIRE(!Error);
				REQUIRE(*Reservation == 1234U);
				*Reservation = 5678;
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}
}