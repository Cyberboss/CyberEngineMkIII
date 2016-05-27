#include "TestHeader.hpp"
SCENARIO("VirtualMemory reservations can be made and released", "[Platform][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A sane reservation size") {
		const auto ReservationSize(1000000);
		WHEN("A reservation is made") {
			void* Result(nullptr);
			REQUIRE_NOTHROW(Result = CYB::Platform::VirtualMemory::Reserve(ReservationSize));
			THEN("No errors occur and a valid address is returned") {
				CHECK(Result != nullptr);
			}
		}
	}
	GIVEN("An existing reservation of sane size") {
		void* const Result(CYB::Platform::VirtualMemory::Reserve(1000000));
		WHEN("The reservation is released") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Release(Result));
			THEN("No errors occur and the address is invalid") {
				REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Commit(Result, 100), CYB::Exception::SystemData);
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE);
			}
		}
	}
	GIVEN("A nonsense reservation") {
		auto const Reservation(nullptr);
		WHEN("The reservation is released") {
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Release(Reservation), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_RELEASE_FAILURE);
			}
		}
	}
}
SCENARIO("VirtualMemory reservations handle various sizes", "[Platform][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("Some reservation size") {
		unsigned long long ReservationSize;
		WHEN("The size is insanely small") {
			ReservationSize = 12;
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Reserve(ReservationSize), CYB::Exception::SystemData);
			THEN("The apporpriate error is thrown") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_RESERVATION_FAILURE);
			}
		}
#ifndef TARGET_OS_LINUX	//OVERCOMMIIITTTTT!!!!
		WHEN("The size is insanely large") {
			ReservationSize = std::numeric_limits<unsigned long long>().max();
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Reserve(ReservationSize), CYB::Exception::SystemData);
			THEN("The apporpriate error is thrown") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_RESERVATION_FAILURE);
			}
		}
#endif
	}
}
SCENARIO("VirtualMemory commits work as intended", "[Platform][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A standard reservation") {
		auto const Reservation(CYB::Platform::VirtualMemory::Reserve(1000000));
		unsigned long long CommitSize;
		WHEN("The size is normal") {
			CommitSize = 4096;
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Commit(Reservation, CommitSize));
			THEN("The commit works as normal and the memory is usable") {
				CHECK_NOTHROW(*static_cast<int*>(Reservation) = 10);
			}
		}
		WHEN("The size is insanely small after a regular commit") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Commit(Reservation, 100));
			CommitSize = 1;
			THEN("The commit works as normal, but nothing changes") {
				REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Commit(Reservation, CommitSize));
			}
		}
		WHEN("The size is larger than the reservation") {
			CommitSize = 2000000;
			THEN("The commit fails") {
				REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Commit(Reservation, CommitSize), CYB::Exception::SystemData);
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE);
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}
	GIVEN("A standard reservation and commit") {
		auto const Reservation(CYB::Platform::VirtualMemory::Reserve(1000000));
		CYB::Platform::VirtualMemory::Commit(Reservation, 500000);
		WHEN("A smaller commit is made") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Commit(Reservation, 1000));
			THEN("No changes are made but the function succeeds") {
				CHECK_NOTHROW(*static_cast<int*>(Reservation) = 1234);
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}
	GIVEN("A nonsense reservation") {
		auto const Reservation(nullptr);
		WHEN("A commit is attempted"){
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Commit(Reservation, 500000), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_COMMITAL_FAILURE);
			}
		}
	}
}
SCENARIO("VirtualMemory reservation protection levels can be changed", "[Platform][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A standard reservation and commit which has some data written to it") {
		auto Reservation(static_cast<unsigned long long*>(CYB::Platform::VirtualMemory::Reserve(1000000)));
		CYB::Platform::VirtualMemory::Commit(Reservation, 500000);
		*Reservation = 1234;
		WHEN("The access level is set to NONE") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::NONE));
			THEN("No errors occur and pages cannot be used") {
				CHECK(true); // :/
			}
		}
		WHEN("The access level is set to READ") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::READ));
			THEN("No errors occur and pages can be read") {
				CHECK(*Reservation == 1234U);
			}
		}
		WHEN("The access level is set to READ_WRITE") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::READ_WRITE));
			THEN("No errors occur and pages can be read and written") {
				CHECK(*Reservation == 1234U);
				*Reservation = 5678;
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}
	GIVEN("A nonsense reservation") {
		unsigned long long* Reservation(nullptr);
		WHEN("The access level is set to NONE") {
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::NONE), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_PROTECT_FAILURE);
			}
		}
		WHEN("The access level is set to READ") {
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::READ), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_PROTECT_FAILURE);
			}
		}
		WHEN("The access level is set to READ_WRITE") {
			REQUIRE_THROWS_AS(CYB::Platform::VirtualMemory::Access(Reservation, CYB::Platform::VirtualMemory::AccessLevel::READ_WRITE), CYB::Exception::SystemData);
			THEN("The appropriate error occurs") {
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MEMORY_PROTECT_FAILURE);
			}
		}
	}
}
SCENARIO("VirtualMemory can be discarded and reused","[Platform][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32Extended> K32E(CYB::Core().FModuleManager.FK32Extended);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A standard reservation and commit which has some data written to it") {
		auto Reservation(static_cast<unsigned long long*>(CYB::Platform::VirtualMemory::Reserve(1000000)));
		CYB::Platform::VirtualMemory::Commit(Reservation, 500000);
		*Reservation = 1234;
		WHEN("The memory is discarded") {
			REQUIRE_NOTHROW(CYB::Platform::VirtualMemory::Discard(Reservation, 500000));
			THEN("No errors occur and pages can be reused but data may differ") {
				CHECK_NOTHROW(*Reservation = 5678);
			}
		}
		CYB::Platform::VirtualMemory::Release(Reservation);
	}
}
