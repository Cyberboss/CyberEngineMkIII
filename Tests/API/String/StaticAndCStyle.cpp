#include "TestHeader.hpp"

SCENARIO("CStyle strings work", "[API][String][CStyle][Unit]") {
	GIVEN("A valid CStyle string") {
		const char* const TestData(u8"asdf");
		WHEN("It is used to instantiate a Static") {
			CYB::API::String::Static TestString(TestData);
			THEN("The byte length is correct") {
				CHECK(TestString.RawLength() == 4);
			}
			THEN("The data matches") {
				CHECK(std::strcmp(TestString.CString(), TestData) == 0);
			}
		}
	}
	GIVEN("Null string data") {
		const char* const TestData(nullptr);
		WHEN("It is used to instantiate a Static") {
			CYB::API::String::Static TestString(TestData);
			THEN("The byte length is correct") {
				CHECK(TestString.RawLength() == 0);
			}
			THEN("The data matches") {
				CHECK(TestString.CString() == nullptr);
			}
		}
	}
}

SCENARIO("Static strings work", "[API][String][Static][Unit]") {
	GIVEN("A valid Static string") {
		const char* const TestData(u8"asdf");
		WHEN("It is used to instantiate a Static") {
			CYB::API::String::Static TestString(TestData);
			THEN("The byte length is correct") {
				CHECK(TestString.RawLength() == 4);
			}
			THEN("The length is correct") {
				CHECK(TestString.Length() == 4);
			}
			THEN("The data matches") {
				CHECK(std::strcmp(TestString.CString(), TestData) == 0);
			}
		}
	}
	GIVEN("Null string data") {
		const char* const TestData(nullptr);
		WHEN("It is used to instantiate a Static") {
			CYB::API::String::Static TestString(TestData);
			THEN("The byte length is correct") {
				CHECK(TestString.RawLength() == 0);
			}
			THEN("The length is correct") {
				CHECK(TestString.Length() == 0);
			}
			THEN("The data matches") {
				CHECK(TestString.CString() == nullptr);
			}
		}
	}
}