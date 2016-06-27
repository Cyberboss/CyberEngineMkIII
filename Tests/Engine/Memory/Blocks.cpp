#include "TestHeader.hpp"

using namespace CYB::Engine::Memory;

SCENARIO("Block initialization performs as expected", "[Engine][Memory][Block][LargeBlock][Unit]"){
	GIVEN("Some valid memory") {
		void* Data(new byte[100]);
		WHEN("A Block is initialized in the memory") {
			auto& TestBlock(*new (Data) Block(100, *static_cast<Block*>(Data), true));
			THEN("The methods report as expected") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(TestBlock.GetData() == static_cast<byte*>(Data) + sizeof(Block));
				Block* Address;
				CHECK_NOTHROW(Address = &Block::FromData(static_cast<byte*>(Data) + sizeof(Block)));
				CHECK(Address == &TestBlock);
				CHECK(TestBlock.IsFree());
				CHECK_FALSE(TestBlock.IsLargeBlock());
				CHECK(static_cast<unsigned long long>(TestBlock.Size()) == 100 - sizeof(Block));
				CHECK(TestBlock.LeftBlock() == nullptr);
				CHECK(reinterpret_cast<byte*>(&TestBlock.RightBlock()) == static_cast<byte*>(Data) + 100);
				CHECK(reinterpret_cast<const byte*>(&static_cast<const Block&>(TestBlock).RightBlock()) == static_cast<byte*>(Data) + 100);
			}
		}
		WHEN("A LargeBlock is initialized in the memory") {
			auto& TestBlock(*new (Data) LargeBlock(100 - sizeof(LargeBlock), nullptr));
			THEN("The methods report as expected") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(TestBlock.GetData() == static_cast<byte*>(Data) + sizeof(Block));
				Block* Address;
				CHECK_NOTHROW(Address = &Block::FromData(static_cast<byte*>(Data) + sizeof(Block)));
				CHECK(Address == &TestBlock);
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.IsLargeBlock());
				CHECK(TestBlock.Size() == 100 - sizeof(LargeBlock));
				CHECK(TestBlock.LeftBlock() == nullptr);
				//RightBlock is HCF on LargeBlocks
			}
		}
		delete[] static_cast<byte*>(Data);
	}
}
SCENARIO("Block dword manipulation works", "[Engine][Memory][Block][LargeBlock][Unit]") {
	auto Data(new byte[100]);
	GIVEN("A valid Block") {
		auto& TestBlock(*new (Data) Block(100, *reinterpret_cast<Block*>(Data), true));
		WHEN("The free bit is set to false") {
			TestBlock.SetFree(false);
			THEN("The dword works as intended") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK_FALSE(TestBlock.IsFree());
				CHECK(TestBlock.Size() == 100 - sizeof(Block));
			}
		}
		WHEN("The free bit is set to true") {
			TestBlock.SetFree(true);
			THEN("The dword works as intended") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.Size() == 100 - sizeof(Block));
			}
		}
		WHEN("The size is changed") {
			TestBlock.SetSize(50);
			THEN("The dword works as intended") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.Size() == 50);
			}
		}
	}
	GIVEN("A valid LargeBlock") {
		auto& TestBlock(*new (Data) LargeBlock(100 - sizeof(LargeBlock), nullptr));
		WHEN("The size is changed") {
			TestBlock.SetSize(50);
			THEN("The size works as intended") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.Size() == 50ULL);
			}
		}
	}
	delete[] Data;
}
SCENARIO("LargeBlock identification works", "[Engine][Memory][Block][LargeBlock][Unit]") {
	auto Data(new byte[100]);
	GIVEN("A valid Block") {
		auto& TestBlock(*new (Data) Block(100, *reinterpret_cast<Block*>(Data), true));
		WHEN("It is checked if it is a LargeBlock") {
			const auto Result(TestBlock.IsLargeBlock());
			THEN("False is returned") {
				CHECK_FALSE(Result);
			}
		}
	}
	GIVEN("A valid LargeBlock") {
		auto& TestBlock(*new (Data) LargeBlock(100 - sizeof(LargeBlock), nullptr));
		WHEN("It is checked if it is a LargeBlock") {
			const auto Result(TestBlock.IsLargeBlock());
			THEN("True is returned") {
				CHECK(Result);
			}
		}
	}
	delete[] Data;
}

SCENARIO("Block Split/Merge functions work", "[Engine][Memory][Block][LargeBlock][Unit]") {
	auto Data(new byte[500]);
	GIVEN("A valid Block") {
		auto& TestBlock(*new (Data) Block(100, *reinterpret_cast<Block*>(Data), true));
		WHEN("It is Spliced") {
			auto& Result(TestBlock.Splice(20));
			REQUIRE(Result.LeftBlock() == &TestBlock);
			REQUIRE(&TestBlock.RightBlock() == &Result);
			THEN("There are now two Blocks with the appropriate settings") {
				CHECK_NOTHROW(Result.Validate());
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(Result.LeftBlock() == &TestBlock);
				CHECK(&TestBlock.RightBlock() == &Result);
				CHECK(TestBlock.Size() == 20);
				CHECK(Result.Size() == 80 - (sizeof(Block) * 2));
				CHECK(TestBlock.IsFree());
				CHECK(Result.IsFree());
				CHECK_FALSE(Result.IsLargeBlock());
				CHECK_FALSE(TestBlock.IsLargeBlock());
			}
		}
		WHEN("It is Spliced and remerged") {
			auto& Tmp(TestBlock.Splice(20));
			REQUIRE(Tmp.LeftBlock() == &TestBlock);
			REQUIRE(&TestBlock.RightBlock() == &Tmp);
			auto& Result(Tmp.EatLeftBlock());
			THEN("There is now one Block with the appropriate settings") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(&Result == &TestBlock);
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.Size() == 100 - sizeof(Block));
				CHECK_FALSE(TestBlock.IsLargeBlock());
			}
		}
	}
	GIVEN("A valid LargeBlock") {
		auto TestBlockP(new (Data) LargeBlock(500 - sizeof(LargeBlock), nullptr));
		WHEN("It is Allocated from") {
			auto& Result(LargeBlock::AllocateBlock(TestBlockP, 20));
			THEN("There are now two Blocks with the appropriate settings") {
				CHECK_NOTHROW(Result.Validate());
				CHECK_NOTHROW(TestBlockP->Validate());
				CHECK(&Result.RightBlock() == TestBlockP);
				CHECK_FALSE(reinterpret_cast<byte*>(TestBlockP) == Data);
				CHECK(reinterpret_cast<byte*>(&Result) == Data);
				CHECK(Result.Size() == 20);
				CHECK_FALSE(Result.IsFree());
				CHECK_FALSE(Result.IsLargeBlock());
				CHECK(TestBlockP->IsLargeBlock());
				CHECK(TestBlockP->Size() == 480 - sizeof(LargeBlock) - sizeof(Block));
				AND_THEN("When it is allocated from again") {
					auto& Result2(LargeBlock::AllocateBlock(TestBlockP, 20));
					THEN("There are now three Blocks with the appropriate settings") {
						CHECK_NOTHROW(Result.Validate());
						CHECK_NOTHROW(TestBlockP->Validate());
						CHECK(&Result.RightBlock() == &Result2);
						CHECK_FALSE(reinterpret_cast<byte*>(TestBlockP) == Data);
						CHECK(reinterpret_cast<byte*>(&Result) == Data);
						CHECK(Result.Size() == 20);
						CHECK_FALSE(Result.IsFree());
						CHECK_FALSE(Result.IsLargeBlock());
						CHECK(TestBlockP->IsLargeBlock());
						CHECK(TestBlockP->Size() == 460 - sizeof(LargeBlock) - (sizeof(Block) * 2));
						CHECK(Result2.LeftBlock() == &Result);
						CHECK(&Result2.RightBlock() == TestBlockP);
						CHECK_FALSE(Result2.IsLargeBlock());
						CHECK_FALSE(Result2.IsFree());
						CHECK(Result2.Size() == 20);
					}
				}
			}
		}
		WHEN("It is Spliced and remerged") {
			auto& Tmp(LargeBlock::AllocateBlock(TestBlockP, 20));
			auto& Result(TestBlockP->EatLeftBlock());
			THEN("There is now one LargeBlock with the appropriate settings") {
				CHECK(&Result == &Tmp);
				CHECK_NOTHROW(Result.Validate());
				CHECK(Result.IsLargeBlock());
				CHECK(Result.Size() == 500 - sizeof(LargeBlock));
			}
		}
	}
	delete[] Data;
}
SCENARIO("Block validation functions work", "[Engine][Memory][Block][LargeBlock][Unit]") {
#ifdef DEBUG
	auto Data(new byte[500]);
	GIVEN("A valid Block") {
		auto& TestBlock(*new (Data) Block(100, *reinterpret_cast<Block*>(Data), true));
		WHEN("It's header is corrupted") {
			*reinterpret_cast<unsigned long long*>(&TestBlock) = 0;
			THEN("The validation will fail") {
				CHECK_THROWS_AS(TestBlock.Validate(), CYB::Exception::Violation);
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_HEAP_BLOCK);
			}
		}
		WHEN("It's footer is corrupted") {
			*(reinterpret_cast<unsigned long long*>(&TestBlock + 1) - 1) = 0;
			THEN("The validation will fail") {
				CHECK_THROWS_AS(TestBlock.Validate(), CYB::Exception::Violation);
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_HEAP_BLOCK);
			}
		}
	}
	GIVEN("A valid LargeBlock") {
		auto& TestBlock(*new (Data) LargeBlock(500 - sizeof(LargeBlock), nullptr));
		WHEN("It's header is corrupted") {
			*reinterpret_cast<unsigned long long*>(reinterpret_cast<byte*>(&TestBlock) + sizeof(Block)) = 0;
			THEN("The validation will fail") {
				CHECK_THROWS_AS(TestBlock.Validate(), CYB::Exception::Violation);
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_HEAP_BLOCK);
			}
		}
		WHEN("It's footer is corrupted") {
			*(reinterpret_cast<unsigned long long*>(&TestBlock + 1) - 1) = 0;
			THEN("The validation will fail") {
				CHECK_THROWS_AS(TestBlock.Validate(), CYB::Exception::Violation);
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_HEAP_BLOCK);
			}
		}
	}
	delete[] Data;
#else
	CHECK(true);
#endif
}