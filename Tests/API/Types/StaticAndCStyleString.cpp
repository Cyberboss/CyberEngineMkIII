#include "TestHeader.hpp"
#include <cstring>

using namespace CYB::API::String;

SCENARIO("CStyle strings work", "[API][Types][String][CStyle][Unit]") {
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

SCENARIO("Static strings work", "[API][Types][String][Static][Unit]") {
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

SCENARIO("String Equivalance works", "[API][Types][String][CStyle][Unit]") {
	GIVEN("Two valid Static strings") {
		CYB::API::String::Static S1("asdf"), S2("asdf"), S3("1234"), S4("12345");
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
		WHEN("S3 and S4 are compared") {
			THEN("They are not equal") {
				CHECK_FALSE(S3 == S4);
			}
		}
	}
}

SCENARIO("CStyle IndexOfByte works", "[API][Types][String][CStyle][Unit]") {
	GIVEN("A valid string") {
		Static Test(u8"abcabcdabcde");
		WHEN("We get the index of a byte that exists") {
			const auto Index(Test.IndexOfByte('b'));
			THEN("It is correct") {
				CHECK(Index == 1);
			}
		}
		WHEN("We get the index of a byte that exists, but we skip a few") {
			const auto Index(Test.IndexOfByte('b', 2));
			THEN("It is correct") {
				CHECK(Index == 8);
			}
		}
		WHEN("We get the index of a byte that doesn't exist") {
			const auto Index(Test.IndexOfByte('f'));
			THEN("It is correct") {
				CHECK(Index == -1);
			}
		}
	}
}