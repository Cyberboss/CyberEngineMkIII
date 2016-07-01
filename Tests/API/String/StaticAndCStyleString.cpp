#include "TestHeader.hpp"
#include <cstring>

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

SCENARIO("String Equivalance works", "[API][String][CStyle][Unit]") {
	GIVEN("Two valid Static strings") {
		CYB::API::String::Static S1("asdf"), S2("asdf"), S3("1234");
		WHEN("S1 and S2 are compared") {
			THEN("They are equal") {
				CHECK(S1 == S2);
			}
		}
		WHEN("S1 and S3 are compared") {
			THEN("They are not equal") {
				CHECK_FALSE(S1 == S3);
			}
		}
		WHEN("S1 and S2 are compared") {
			THEN("They are not unequal") {
				CHECK_FALSE(S1 != S2);
			}
		}
		WHEN("S1 and S3 are compared") {
			THEN("They are unequal") {
				CHECK(S1 != S3);
			}
		}
	}
}