#include "TestHeader.hpp"

SCENARIO("Dynamic string constructors work","[API][String][Dynamic][Unit]") {
	GIVEN("Two static strings") {
		CYB::API::String::Static S1("asdf"), S2("qwerty");
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
	}
}