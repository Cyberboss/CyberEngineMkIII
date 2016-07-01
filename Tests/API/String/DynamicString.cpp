#include "TestHeader.hpp"

using namespace CYB::API::String;

SCENARIO("Dynamic string constructors work","[API][String][Dynamic][Unit]") {
	GIVEN("Two static strings") {
		CYB::API::String::Static S1("asdf"), S2("qwerty"), S3;
		WHEN("A basic Dynamic string is constructed") {
			CYB::API::String::Dynamic D1;
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("A Dynamic string is constructed from a static string") {
			CYB::API::String::Dynamic D2(S1);
			THEN("All is well") {
				CHECK(true);
			}
			AND_THEN("When it is copied") {
				CYB::API::String::Dynamic D3(D2);
				THEN("All is well") {
					CHECK(true);
				}
			}
			AND_THEN("When it is moved") {
				CYB::API::String::Dynamic D4(std::move(D2));
				THEN("All is well") {
					CHECK(true);
				}
			}
			AND_THEN("When it is moved assigned to an existing dynamic string") {
				CYB::API::String::Dynamic D5(S2);
				D5 = std::move(D2);
				THEN("All is well") {
					CHECK(true);
				}
			}
		}
		WHEN("A Dynamic string is constructed from an empty static string") {
			CYB::API::String::Dynamic D6(S3);
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Dynamic string Length works", "[API][String][Dynamic][Unit]") {
	GIVEN("A valid Dynamic string") {
		CYB::API::String::Dynamic D7("asdf");
		WHEN("The length is checked") {
			const auto Result(D7.Length());
			THEN("It is correct") {
				CHECK(Result == 4);
			}
		}
	}
}

SCENARIO("Dynamic string Shrink works", "[API][String][Dynamic][Unit]") {
	GIVEN("A valid Dynamic string") {
		CYB::API::String::Dynamic D7("asdf");
		WHEN("It is shrunk normally") {
			D7.Shrink(2);
			THEN("It is correct") {
				Static Test("as");
				CHECK(D7 == Test);
			}
		}
		WHEN("It is shrunk oversized") {
			const auto Compare(D7);
			D7.Shrink(6);
			THEN("Nothing happens") {
				CHECK(Compare == D7);
			}
		}
		WHEN("It is shrunk to zero") {
			D7.Shrink(0);
			THEN("It is nullified") {
				CHECK(D7 == Dynamic());
			}
		}
	}
}