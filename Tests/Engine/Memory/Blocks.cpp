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
			}
		}
		delete[] static_cast<byte*>(Data);
	}
}