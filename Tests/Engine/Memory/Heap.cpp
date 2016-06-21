#include "TestHeader.hpp"

using namespace CYB::Engine::Memory;

SCENARIO("Heap Alloc works", "[Engine][Memory][Heap][Functional]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap") {
		Heap TestHeap(10000);
		WHEN("A sane allocation is made") {
			void* Result(nullptr);
			REQUIRE_NOTHROW(Result = TestHeap.Alloc(50));
			THEN("A valid, allocated, aligned memory region is returned") {
				CHECK(Result != nullptr);
				CHECK_FALSE(Block::FromData(Result).IsLargeBlock());
				CHECK_FALSE(Block::FromData(Result).IsFree());
				CHECK(Block::FromData(Result).Size() >= 50);
				CHECK(Block::FromData(Result).Size() % sizeof(void*) == 0U);
				CHECK(reinterpret_cast<unsigned long long>(Result) % sizeof(void*) == 0U);
			}
		}
		WHEN("An zero allocation is made") {
			void* Result(nullptr);
			REQUIRE_NOTHROW(Result = TestHeap.Alloc(0));
			THEN("nullptr is returned") {
				CHECK(Result == nullptr);
			}
		}
		WHEN("An allocation of unsigned size is made") {
			void* Result(nullptr);
			REQUIRE_THROWS_AS(Result = TestHeap.Alloc(-5), CYB::Exception::Violation);
			THEN("The appropriate exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
				CHECK(Result == nullptr);
			}
		}
	}
}

SCENARIO("Heap Realloc works", "[Engine][Memory][Heap][Functional]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap") {
		Heap TestHeap(10000);
		WHEN("A sane allocation is made over null") {
			void* Result(nullptr);
			REQUIRE_NOTHROW(Result = TestHeap.Alloc(50));
			THEN("A valid, allocated, aligned memory region is returned") {
				CHECK(Result != nullptr);
				CHECK_FALSE(Block::FromData(Result).IsLargeBlock());
				CHECK_FALSE(Block::FromData(Result).IsFree());
				CHECK(Block::FromData(Result).Size() >= 50);
				CHECK(Block::FromData(Result).Size() % sizeof(void*) == 0U);
				CHECK(reinterpret_cast<unsigned long long>(Result) % sizeof(void*) == 0U);
			}
		}
	}
}