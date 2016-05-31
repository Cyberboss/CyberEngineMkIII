#include "TestHeader.hpp"

using namespace CYB::Engine;

SCENARIO("Test that Block initialization performs as expected", "[Memory][Unit]"){
	GIVEN("Some valid memory") {
		void* Data(new byte[100]);
		WHEN("A Block is initialized in the memory") {
			auto& TestBlock(*new (Data) CYB::Engine::Block(100, *static_cast<Block*>(Data), true));
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
				CHECK(reinterpret_cast<byte*>(TestBlock.RightBlock()) == static_cast<byte*>(Data) + 100);
			}
		}
		WHEN("A LargeBlock is initialized in the memory") {
			auto& TestBlock(*new (Data) CYB::Engine::LargeBlock(100 - sizeof(LargeBlock)));
			THEN("The methods report as expected") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(TestBlock.GetData() == static_cast<byte*>(Data) + sizeof(Block));
				Block* Address;
				CHECK_NOTHROW(Address = &Block::FromData(static_cast<byte*>(Data) + sizeof(Block)));
				CHECK(Address == &TestBlock);
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.IsLargeBlock());
				CHECK(TestBlock.Size() == 0);
				CHECK(TestBlock.LeftBlock() == nullptr);
				//RightBlock is HCF on LargeBlocks
			}
		}
		delete[] static_cast<byte*>(Data);
	}
}
SCENARIO("Test that Block dword manipulation works", "[Memory][Unit]") {
	GIVEN("A valid Block") {
		auto Data(new byte[100]);
		auto& TestBlock(*new (Data) CYB::Engine::Block(100, *reinterpret_cast<Block*>(Data), true));
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
		delete[] Data;
	}
}
SCENARIO("Test that LargeBlock identification works", "[Memory][Unit]") {
	GIVEN("A valid Block") {
		auto Data(new byte[100]);
		auto& TestBlock(*new (Data) CYB::Engine::Block(100, *reinterpret_cast<Block*>(Data), true));
		WHEN("It is checked if it is a LargeBlock") {
			const auto Result(TestBlock.IsLargeBlock());
			THEN("False is returned") {
				CHECK_FALSE(Result);
			}
		}
		delete[] Data;
	}
	GIVEN("A valid LargeBlock") {
		auto Data(new byte[100]);
		auto& TestBlock(*new (Data) CYB::Engine::LargeBlock(100 - sizeof(LargeBlock)));
		WHEN("It is checked if it is a LargeBlock") {
			const auto Result(TestBlock.IsLargeBlock());
			THEN("True is returned") {
				CHECK(Result);
			}
		}
		delete[] Data;
	}
}

SCENARIO("Test Block Split/Merge functions work", "[Memory][Unit]") {
	GIVEN("A valid Block") {
		auto Data(new byte[100]);
		auto& TestBlock(*new (Data) CYB::Engine::Block(100, *reinterpret_cast<Block*>(Data), true));
		WHEN("It is Spliced") {
			auto& Result(TestBlock.Splice(20));
			THEN("There are now two Blocks with the appropriate settings") {
				CHECK_NOTHROW(Result.Validate());
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(Result.LeftBlock() == &TestBlock);
				CHECK(TestBlock.RightBlock() == &Result);
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
			auto& Result(Tmp.EatLeftBlock());
			THEN("There are now two Blocks with the appropriate settings") {
				CHECK_NOTHROW(TestBlock.Validate());
				CHECK(&Result == &TestBlock);
				CHECK(TestBlock.IsFree());
				CHECK(TestBlock.Size() == 100 - sizeof(Block));
				CHECK_FALSE(TestBlock.IsLargeBlock());
			}
		}
		delete[] Data;
	}
}