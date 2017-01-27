#include "TestHeader.hpp"

SCENARIO("Allocatables can be allocated", "[API][Interop][Allocator][Unit]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	ModuleDependancy<CYB::Platform::Modules::PThread> PThread;
	ModuleDependancy<CYB::Platform::Modules::LibC> LibC;
	ModuleDependancy<CYB::Platform::Modules::RT> RT;
	ModuleDependancy<CYB::Platform::Modules::System> System;
	auto& Alloc(static_cast<CYB::Engine::Allocator&>(CYB::API::Context().FAllocator));
	GIVEN("Abstract allocatables") {
		using namespace CYB::API;
		WHEN("We try to allocate a Mutex") {
			using TestAllocatable = Mutex;
			REQUIRE_NOTHROW(Alloc.NewObject<TestAllocatable>());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We try to allocate a Path") {
			using TestAllocatable = Path;
			REQUIRE_NOTHROW(Alloc.NewObject<TestAllocatable>(Path::SystemPath::TEMPORARY));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
	GIVEN("Concrete allocatables") {
		using namespace CYB::Platform::System;
		WHEN("We try to allocate a Mutex") {
			using TestAllocatable = Mutex;
			REQUIRE_NOTHROW(Alloc.NewObject<TestAllocatable>());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We try to allocate a Path") {
			using TestAllocatable = Path;
			REQUIRE_NOTHROW(Alloc.NewObject<TestAllocatable>(Path::SystemPath::TEMPORARY));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}


class FakeConstructor : public CYB::API::Interop::EmptyConstructor {
public:
	FakeConstructor() :
		CYB::API::Interop::EmptyConstructor(99U)
	{}
};

template <> void CYB::API::Interop::Allocator::Backdoor<FakeConstructor>(FakeConstructor& AHooker) {
	CYB::API::Context().FAllocator.InteropAllocation(Allocatable::GetID<Mutex>(), AHooker);
}

SCENARIO("Allocator invalid constructor error is actually invoked", "[API][Interop][Allocator][Unit]") {
#ifdef DEBUG
	GIVEN("A bad constructor for Mutex") {
		FakeConstructor Bad;
		WHEN("An interop allocation is invoked with this constructor") {
			REQUIRE_THROWS_AS(CYB::API::Interop::Allocator::Backdoor(Bad), CYB::Exception::Violation);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_INTEROP_CONSTRUCTOR);
			}
		}
	}
#else
	CHECK(true);
#endif
}