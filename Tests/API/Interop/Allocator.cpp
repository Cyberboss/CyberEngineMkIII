#include "TestHeader.hpp"

SCENARIO("Allocatables can be allocated", "[API][Interop][Allocator][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::LINUX, CYB::Platform::Modules::AMRT> RT(CYB::Core().FModuleManager.FRT);
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMSystem> System(CYB::Core().FModuleManager.FSystem);
	GIVEN("Abstract allocatables") {
		using namespace CYB::API;
		WHEN("We try to allocate a Mutex") {
			using TestAllocatable = Mutex;
			REQUIRE_NOTHROW(CYB::API::Context().FAllocator.NewObject<TestAllocatable>());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We try to allocate a Path") {
			using TestAllocatable = Path;
			REQUIRE_NOTHROW(CYB::API::Context().FAllocator.NewObject<TestAllocatable>(Path::SystemPath::TEMPORARY));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
	GIVEN("Concrete allocatables") {
		using namespace CYB::Platform::System;
		WHEN("We try to allocate a Mutex") {
			using TestAllocatable = Mutex;
			REQUIRE_NOTHROW(CYB::API::Context().FAllocator.NewObject<TestAllocatable>());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We try to allocate a Path") {
			using TestAllocatable = Path;
			REQUIRE_NOTHROW(CYB::API::Context().FAllocator.NewObject<TestAllocatable>(Path::SystemPath::TEMPORARY));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}