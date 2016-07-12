#include "TestHeader.hpp"

SCENARIO("STLAllocator works", "[API][Interop][STLAllocator][Unit]") {

	struct YourMother {
		char FA;
		short FB;
		int FC;
		long long FD;
	};

	GIVEN("An allocator") {
		CYB::API::Interop::STLAllocator<YourMother> TestAllocator;
		static_cast<void>(TestAllocator);
		WHEN("A thing is allocated from it") {
			YourMother* Result(nullptr);
			REQUIRE_NOTHROW(Result = TestAllocator.allocate(3));
			THEN("A valid result is returned") {
				REQUIRE(Result != nullptr);
				AND_WHEN("The result is freed, all is well") {
					CHECK_NOTHROW(TestAllocator.deallocate(Result, 3));
				}
			}
		}
	}

	GIVEN("A deque using the allocator") {
		typedef CYB::API::Container::Deque<YourMother> TheDeque;
		WHEN("It is used") {
			const auto Test([](){
				TheDeque asdf{ YourMother(), YourMother(), YourMother{1,1,1,1} };
				asdf.push_back(YourMother());
				asdf.pop_front();
			});
			REQUIRE_NOTHROW(Test());
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}