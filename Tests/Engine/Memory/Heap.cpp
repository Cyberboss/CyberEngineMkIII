#include "TestHeader.hpp"

#include <deque>
#include <random>

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
		WHEN("An allocation of unsigned/negative size is made") {
			void* Result(nullptr);
			REQUIRE_THROWS_AS(Result = TestHeap.Alloc(-5), CYB::Exception::Violation);
			THEN("The appropriate exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::NEGATIVE_HEAP_ALLOCATION);
				CHECK(Result == nullptr);
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
		auto Allocation(TestHeap.Alloc(50));
		WHEN("The heap is walked") {
			REQUIRE_NOTHROW(TestHeap.Walk());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("The allocation is corrupted") {
			*reinterpret_cast<int*>(&Block::FromData(Allocation)) = 0;
			AND_THEN("The heap is walked") {
				REQUIRE_THROWS_AS(TestHeap.Walk(), CYB::Exception::Violation);
				THEN("The appropriate error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_HEAP_BLOCK);
				}
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
		WHEN("A corrupted block is freed") {
			auto Data(TestHeap.Alloc(50));
			*reinterpret_cast<int*>(&Block::FromData(Data)) = 6;
			REQUIRE_THROWS_AS(TestHeap.Free(Data), CYB::Exception::Violation);
			THEN("The appropriate error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_HEAP_BLOCK);
			}
		}
	}
}

SCENARIO("Heap seeded random stress test", "[Engine][Memory][Heap][Functional][Stress]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("A basic heap and rng seed") {
		Heap TestHeap(1000);
		std::mt19937 RNG(3);

		WHEN("We execute 10000 de/re/allocations") {

			enum AllocType : unsigned int{
				ALLOC1,
				ALLOC2,
				ALLOC3,
				REALLOC1,
				REALLOC2,
				FREE,
			};
			std::deque<void*> Allocations;
			for (unsigned int I(0); I < 10000; ++I) {
				if (I == 0x12)
					BREAK;
				const auto Type(static_cast<AllocType>(Allocations.empty() ? ALLOC1 : RNG() % 6));
				switch (Type) {
				case ALLOC1:
				case ALLOC2:
				case ALLOC3:
					Allocations.emplace_back(TestHeap.Alloc(static_cast<int>(RNG() % 1000) + 1));
					break;
				case FREE:
				{
					auto Pos(RNG() % Allocations.size());
					TestHeap.Free(Allocations[Pos]);
					Allocations.erase(Allocations.begin() + static_cast<long long>(Pos));
					break;
				}
				case REALLOC1:
				case REALLOC2:
				{
					auto Pos(RNG() % Allocations.size());
					auto& B(Block::FromData(Allocations[Pos]));
					Allocations[Pos] = TestHeap.Realloc(Allocations[Pos], std::max(static_cast<int>(B.Size()), static_cast<int>(RNG() % 1000)) + 1);
					break;
				}
				}
				REQUIRE_NOTHROW(TestHeap.Walk());
			}
			for (auto Alloc : Allocations)
				TestHeap.Free(Alloc);

			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}