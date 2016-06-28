#include "TestHeader.hpp"

#include <cstring>

SCENARIO("UTF8 string constructors work","[API][String][UTF8][Unit]") {
	GIVEN("Two static strings") {
		CYB::API::String::Static S1(u8"asdf"), S2(u8"qwerty"), S3;
		WHEN("A basic UTF8 string is constructed") {
			CYB::API::String::Dynamic D1;
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("A UTF8 string is constructed from a static string") {
			CYB::API::String::UTF8 D2;
			D2 = CYB::API::String::UTF8(S1);
			THEN("All is well") {
				CHECK(true);
			}
			AND_THEN("When it is copied") {
				CYB::API::String::UTF8 D3(D2);
				THEN("All is well") {
					CHECK(true);
				}
			}
			AND_THEN("When it is moved") {
				CYB::API::String::UTF8 D4(std::move(D2));
				THEN("All is well") {
					CHECK(true);
				}
			}
			AND_THEN("When it is moved assigned to an existing UTF8 string") {
				CYB::API::String::UTF8 D5(S2);
				D5 = std::move(D2);
				THEN("All is well") {
					CHECK(true);
				}
			}
		}
		WHEN("A UTF8 string is constructed from an empty static string") {
			CYB::API::String::UTF8 D6;
			REQUIRE_NOTHROW(D6 = CYB::API::String::UTF8(S3));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}
SCENARIO("UTF8 string Length and validation works", "[API][String][Dynamic][Unit]") {
	GIVEN("A valid UTF8 string") {
		auto Data(u8"私は自分のベストを尽くします");
		CYB::API::String::Static S4(Data);
		CYB::API::String::UTF8 D7;
		REQUIRE_NOTHROW(D7 = CYB::API::String::UTF8(S4));
		WHEN("It is validated") {
			const auto Result(CYB::API::String::UTF8::Validate(S4));
			THEN("The result is correct") {
				CHECK(Result);
			}
		}
		WHEN("The length is checked") {
			const auto Result(D7.Length()), Raw(D7.RawLength());
			THEN("It is correct") {
				CHECK(Result == 14);
				CHECK(Raw == 42);
			}
		}
		WHEN("The nth character is checked") {
			const auto Result(&D7[1]);
			THEN("It matches") {
				auto Data2(u8"は自分のベストを尽くします");
				CHECK(strcmp(Data2, Result) == 0);
			}
		}
	}
	GIVEN("An invalid UTF8 string") {
		auto Wide(L"私は自分のベストを尽くします");
		const void* Alias(Wide);
		CYB::API::String::Static S4(static_cast<const char*>(Alias));
		WHEN("It is validated") {
			const auto Result(CYB::API::String::UTF8::Validate(S4));
			THEN("The result is correct") {
				CHECK_FALSE(Result);
			}
		}
		WHEN("A UTF8 is attempted construction from it") {
			CYB::API::String::UTF8 D7;
			CHECK_THROWS_AS(D7 = CYB::API::String::UTF8(S4), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
			}
		}
	}
}