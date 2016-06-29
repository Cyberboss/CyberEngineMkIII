#include "TestHeader.hpp"

#include <deque>
#include <random>

using namespace CYB::Engine::Memory;

REDIRECTED_FUNCTION(BadVirtualAlloc, void* const, const unsigned long long, const unsigned long, const unsigned long) {
	return static_cast<void*>(nullptr);
}

REDIRECTED_FUNCTION(BadMProtect, void* const, const unsigned long long, const int) {
	return -1;
}

SCENARIO("Heap Alloc works", "[Engine][Memory][Heap][Functional]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap") {
		Heap TestHeap(5);
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
		WHEN("An allocation of unsigned/negative size is made") {
			void* Result(nullptr);
			REQUIRE_THROWS_AS(Result = TestHeap.Alloc(-5), CYB::Exception::Violation);
			THEN("The appropriate exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
				CHECK(Result == nullptr);
			}
		}
		WHEN("An allocation that is too large is made") {
			void* Result(nullptr);
			REQUIRE_THROWS_AS(Result = TestHeap.Alloc(std::numeric_limits<int>::max()), CYB::Exception::Violation);
			THEN("The appropriate exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::UNSUPPORTED_ALLOCATION_AMOUNT);
				CHECK(Result == nullptr);
			}
		}
		GIVEN("A failing Reserve call") {
			auto BVA(K32.Redirect<CYB::Platform::Modules::Kernel32::VirtualAlloc, BadVirtualAlloc>());
			auto BMP(LibC.Redirect<CYB::Platform::Modules::LibC::mprotect, BadMProtect>());
			WHEN("Heap expansion is attempted") {
				void* Result(nullptr);
				REQUIRE_THROWS_AS(Result = TestHeap.Alloc(1000), CYB::Exception::SystemData);
				THEN("The appropriate exception is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE);
					CHECK(Result == nullptr);
				}
			}
		}
	}
}

SCENARIO("Heap walk works", "[Engine][Memory][Heap][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap and allocation") {
		Heap TestHeap(10000);
		TestHeap.Alloc(50);
		WHEN("The heap is walked") {
			REQUIRE_NOTHROW(TestHeap.Walk());
			THEN("All is well") {
				CHECK(true);
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
		GIVEN("A nullptr start") {
			auto Base(nullptr);
			WHEN("A sane reallocation is made") {
				void* Result(nullptr);
				REQUIRE_NOTHROW(Result = TestHeap.Realloc(Base, 50));
				THEN("The result is the same as a call to Alloc") {
					CHECK(Result != nullptr);
					CHECK_FALSE(Block::FromData(Result).IsLargeBlock());
					CHECK_FALSE(Block::FromData(Result).IsFree());
					CHECK(Block::FromData(Result).Size() >= 50);
					CHECK(Block::FromData(Result).Size() % sizeof(void*) == 0U);
					CHECK(reinterpret_cast<unsigned long long>(Result) % sizeof(void*) == 0U);
				}
			}
			WHEN("An zero reallocation is made") {
				void* Result(nullptr);
				REQUIRE_NOTHROW(Result = TestHeap.Realloc(Base, 0));
				THEN("nullptr is returned") {
					CHECK(Result == nullptr);
				}
			}
			WHEN("An allocation of unsigned/negative size is made") {
				void* Result(nullptr);
				REQUIRE_THROWS_AS(Result = TestHeap.Realloc(Base, -5), CYB::Exception::Violation);
				THEN("The appropriate exception is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
					CHECK(Result == nullptr);
				}
			}
		}
		GIVEN("A basic allocation start") {
			auto Base(TestHeap.Alloc(50));
			WHEN("A greater reallocation is made") {
				void* Result(nullptr);
				REQUIRE_NOTHROW(Result = TestHeap.Realloc(Base, 60));
				THEN("The result is correct") {
					CHECK(Result != nullptr);
					CHECK_FALSE(Block::FromData(Result).IsLargeBlock());
					CHECK_FALSE(Block::FromData(Result).IsFree());
					CHECK(Block::FromData(Result).Size() >= 60);
					CHECK(Block::FromData(Result).Size() % sizeof(void*) == 0U);
					CHECK(reinterpret_cast<unsigned long long>(Result) % sizeof(void*) == 0U);
					AND_THEN("Since the heap was empty, the result should be equivalent to the base") {
						CHECK(Result == Base);
					}
				}
			}
			WHEN("An lesser reallocation is made") {
				void* Result(nullptr);
				REQUIRE_NOTHROW(Result = TestHeap.Realloc(Base, 30));
				THEN("The same block is returned, unmodified") {
					CHECK(Result != nullptr);
					CHECK(Result == Base);
					CHECK_FALSE(Block::FromData(Result).IsLargeBlock());
					CHECK_FALSE(Block::FromData(Result).IsFree());
					CHECK(Block::FromData(Result).Size() >= 50);
					CHECK(Block::FromData(Result).Size() % sizeof(void*) == 0U);
					CHECK(reinterpret_cast<unsigned long long>(Result) % sizeof(void*) == 0U);
				}
			}
			WHEN("An allocation of unsigned/negative size is made") {
				void* Result(nullptr);
				REQUIRE_THROWS_AS(Result = TestHeap.Realloc(Base, -5), CYB::Exception::Violation);
				THEN("The appropriate exception is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
					CHECK(Result == nullptr);
				}
			}
			WHEN("A zero allocation is made") {
				void* Result(nullptr);
				REQUIRE_NOTHROW(Result = TestHeap.Realloc(Base, 0));
				THEN("The block has been freed") {
					CHECK(Result == nullptr);
				}
			}
			WHEN("An allocation that is too large is made") {
				void* Result(nullptr);
				REQUIRE_THROWS_AS(Result = TestHeap.Realloc(Base, std::numeric_limits<int>::max()), CYB::Exception::Violation);
				THEN("The appropriate exception is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::UNSUPPORTED_ALLOCATION_AMOUNT);
					CHECK(Result == nullptr);
				}
			}
		}
	}
}

SCENARIO("Heap Free works", "[Engine][Memory][Heap][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap") {
		Heap TestHeap(10000);
		WHEN("nullptr is freed") {
			TestHeap.Free(nullptr);
			THEN("Nothing happens") {
				CHECK(true);
			}
		}
		WHEN("A basic allocation is freed") {
			TestHeap.Free(TestHeap.Alloc(50));
			THEN("Nothing happens") {
				CHECK(true);
			}
		}
		WHEN("A reallocation is freed") {
			TestHeap.Free(TestHeap.Realloc(TestHeap.Alloc(50), 60));
			THEN("Nothing happens") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Heap seeded random stress test", "[Engine][Memory][Heap][Functional][Stress][Slow]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap and rng seed") {
		Heap TestHeap(1000);
		std::mt19937 RNG(3);

		WHEN("We execute 10000 de/re/allocations") {

			enum AllocType : unsigned int{
				ALLOC,
				ALLOC2,
				ALLOC3,
				REALLOC,
				REALLOC2,
				FREE,
			};
			struct Allocation {
				void* FData;
				unsigned int FTimesRealloced;
			};

			std::deque<Allocation> Allocations;
			for (unsigned int I(0); I < 10000; ++I) {
				const auto Type(static_cast<AllocType>(Allocations.empty() ? ALLOC : RNG() % 6));
				switch (Type) {
				case ALLOC:
				case ALLOC2:
				case ALLOC3:
					Allocations.push_back({ TestHeap.Alloc(static_cast<int>(RNG() % 1000) + 1), 0 });
					break;
				case FREE:
				{
					auto Pos(RNG() % Allocations.size());
					TestHeap.Free(Allocations[Pos].FData);
					Allocations.erase(Allocations.begin() + static_cast<long long>(Pos));
					break;
				}
				case REALLOC:
				case REALLOC2:
				{
					auto Pos(RNG() % Allocations.size());
					auto& B(Block::FromData(Allocations[Pos].FData));
					Allocations[Pos].FData = TestHeap.Realloc(Allocations[Pos].FData, std::max(static_cast<int>(B.Size()), static_cast<int>(RNG() % 1000)) + 1);
					++Allocations[Pos].FTimesRealloced;
					break;
				}
				}
				REQUIRE_NOTHROW(TestHeap.Walk());
			}
			for (auto Alloc : Allocations)
				TestHeap.Free(Alloc.FData);

			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}