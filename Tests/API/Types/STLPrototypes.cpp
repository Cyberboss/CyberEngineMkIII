#include "TestHeader.hpp"
SCENARIO("UniquePointers work", "[API][Types][STL][Unit]") {
	GIVEN("Something to point to") {
		class Something {};
		auto Pointer(new (CYB::API::Context().FAllocator.FHeap.Alloc(sizeof(Something))) Something());
		WHEN("We create a UniquePointer for it") {
			CYB::API::UniquePointer<Something> Kajigger(Pointer);
			THEN("It is freed correctly") {
				CHECK(true);
			}
		}
		WHEN("We assign to it after construction") {
			CYB::API::UniquePointer<Something> Kajigger;
			Kajigger.reset(Pointer);
			THEN("It is freed correctly") {
				CHECK(true);
			}
		}
		WHEN("We assign to it after null construction") {
			CYB::API::UniquePointer<Something> Kajigger(nullptr);
			Kajigger.reset(Pointer);
			THEN("It is freed correctly") {
				CHECK(true);
			}
		}
	}
}