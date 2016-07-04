#include "TestHeader.hpp"
SCENARIO("VirtualMemory reservations can be made and released", "[Platform][System][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A sane reservation size") {
		const auto ReservationSize(1000000);
		WHEN("A reservation is made") {
			CYB::Platform::System::VirtualMemory* Result(nullptr);
			REQUIRE_NOTHROW(Result = new CYB::Platform::System::VirtualMemory(ReservationSize));
			THEN("No errors occur and a valid address is returned") {
				CHECK_FALSE(Result == nullptr);
				CHECK((*Result)() != nullptr);
			}
			delete Result;
		}
	}
	GIVEN("An existing reservation of sane size") {
		CYB::Platform::System::VirtualMemory* Result = new CYB::Platform::System::VirtualMemory(1000000);
		WHEN("The reservation is released") {
			REQUIRE_NOTHROW(Result->~VirtualMemory());
			THEN("All is well") {
				CHECK(true);
			}
		}
		operator delete[](Result);
	}
}
SCENARIO("VirtualMemory commits work as intended", "[Platform][System][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A standard reservation") {
		CYB::Platform::System::VirtualMemory Reservation(1000000);
		unsigned long long CommitSize;
		WHEN("The size is normal") {
			CommitSize = 4096;
			REQUIRE_NOTHROW(Reservation.Commit( CommitSize));
			THEN("The commit works as normal and the memory is usable") {
				CHECK_NOTHROW(*static_cast<int*>(Reservation()) = 10);
			}
		}
		WHEN("The size is insanely small after a regular commit") {
			REQUIRE_NOTHROW(Reservation.Commit(100));
			CommitSize = 1;
			THEN("The commit works as normal, but nothing changes") {
				REQUIRE_NOTHROW(Reservation.Commit(CommitSize));
			}
		}
		WHEN("The size is larger than the reservation") {
			CommitSize = 2000000;
			THEN("The commit fails") {
				REQUIRE_THROWS_AS(Reservation.Commit(CommitSize), CYB::Exception::Internal);
				CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MEMORY_COMMITAL_FAILURE);
			}
		}
	}
	GIVEN("A standard reservation and commit") {
		CYB::Platform::System::VirtualMemory Reservation(1000000);
		Reservation.Commit(500000);
		WHEN("A smaller commit is made") {
			REQUIRE_NOTHROW(Reservation.Commit(1000));
			THEN("No changes are made but the function succeeds") {
				CHECK_NOTHROW(*static_cast<int*>(Reservation()) = 1234);
			}
		}
	}
}
SCENARIO("VirtualMemory reservation protection levels can be changed", "[Platform][System][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A standard reservation and commit which has some data written to it") {
		CYB::Platform::System::VirtualMemory Reservation(1000000);
		Reservation.Commit(500000);
		*static_cast<unsigned long long*>(Reservation()) = 1234;
		WHEN("The access level is set to NONE") {
			REQUIRE_NOTHROW(Reservation.Access(CYB::Platform::System::VirtualMemory::AccessLevel::NONE));
			THEN("No errors occur and pages cannot be used") {
				CHECK(true);
			}
		}
		WHEN("The access level is set to READ") {
			REQUIRE_NOTHROW(Reservation.Access(CYB::Platform::System::VirtualMemory::AccessLevel::READ));
			THEN("No errors occur and pages can be read") {
				CHECK(*static_cast<unsigned long long*>(Reservation()) == 1234U);
			}
		}
		WHEN("The access level is set to READ_WRITE") {
			REQUIRE_NOTHROW(Reservation.Access(CYB::Platform::System::VirtualMemory::AccessLevel::READ_WRITE));
			THEN("No errors occur and pages can be read and written") {
				CHECK(*static_cast<unsigned long long*>(Reservation()) == 1234U);
				*static_cast<unsigned long long*>(Reservation()) = 5678;
			}
		}
	}
}
SCENARIO("VirtualMemory can be discarded and reused","[Platform][System][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32Extended> K32E(CYB::Core().FModuleManager.FK32Extended);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A standard reservation and commit which has some data written to it") {
		CYB::Platform::System::VirtualMemory Reservation(1000000);
		Reservation.Commit(500000);
		*static_cast<unsigned long long*>(Reservation()) = 1234;
		WHEN("The memory is discarded") {
			REQUIRE_NOTHROW(Reservation.Discard(Reservation(), 500000));
			THEN("No errors occur and pages can be reused but data may differ") {
				CHECK_NOTHROW(*static_cast<unsigned long long*>(Reservation()) = 5678);
			}
		}
	}
}

REDIRECTED_FUNCTION(BadVirtualAlloc, void* const, const unsigned long long, const unsigned long, const unsigned long) {
	return static_cast<void*>(nullptr);
}

REDIRECTED_FUNCTION(BadVirtualFree, void* const, const unsigned long long, const unsigned long) {
	return 0;
}

REDIRECTED_FUNCTION(BadVirtualProtect, void* const, const unsigned long long, const unsigned long, unsigned long*) {
	return 0;
}

REDIRECTED_FUNCTION(BadMMap, void* const, const unsigned long long, int, int, int, const unsigned long long) {
	return reinterpret_cast<void*>(-1);
}

REDIRECTED_FUNCTION(BadMUnmap, void* const, const unsigned long long) {
	return -1;
}
REDIRECTED_FUNCTION(BadMProtect, void* const, const unsigned long long, const int) {
	return -1;
}

SCENARIO("VirtualMemory errors work", "[Platform][System][VirtualMemory][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32Extended> K32E(CYB::Core().FModuleManager.FK32Extended);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("Calls that will result in error and a valid reservation and commit") {
		CYB::Platform::System::VirtualMemory ValidReservation(1000000);
		ValidReservation.Commit(1000);
		{
			auto BVA(K32.Redirect<CYB::Platform::Modules::Kernel32::VirtualAlloc, BadVirtualAlloc>());
			auto BVF(K32.Redirect<CYB::Platform::Modules::Kernel32::VirtualFree, BadVirtualFree>());
			auto BMM(LibC.Redirect<CYB::Platform::Modules::LibC::mmap, BadMMap>());
			auto BMU(LibC.Redirect<CYB::Platform::Modules::LibC::munmap, BadMUnmap>());
			auto BMP(LibC.Redirect<CYB::Platform::Modules::LibC::mprotect, BadMProtect>());
			WHEN("A reservation is attempted") {
				CYB::Platform::System::VirtualMemory* Reservation(nullptr);
				REQUIRE_THROWS_AS(Reservation = new CYB::Platform::System::VirtualMemory(10000), CYB::Exception::Internal);
				THEN("The appropriate exception occurs") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MEMORY_RESERVATION_FAILURE);
				}
				delete Reservation;
			}
			WHEN("A commit is attempted on valid memory") {
				REQUIRE_THROWS_AS(ValidReservation.Commit(10000), CYB::Exception::Internal);
				THEN("The appropriate exception occurs") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MEMORY_COMMITAL_FAILURE);
				}
			}
			WHEN("An access change is attempted on valid memory") {
				auto BVP(K32.Redirect<CYB::Platform::Modules::Kernel32::VirtualProtect, BadVirtualProtect>());	//Access is used before commit on windows but it fails in the same way
				REQUIRE_THROWS_AS(ValidReservation.Access(CYB::Platform::System::VirtualMemory::AccessLevel::READ), CYB::Exception::Internal);
				THEN("The appropriate exception occurs") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MEMORY_PROTECT_FAILURE);
				}
			}
		}
	}
}