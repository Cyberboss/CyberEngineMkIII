#include "TestHeader.hpp"

SCENARIO("Exception error code verification works", "[Exception][Unit]") {
	GIVEN("A fake exception code") {
		const auto FakeExceptionCode(static_cast<unsigned int>(-1));
		WHEN("A Violation instantiation is attempted") {
			REQUIRE_THROWS_AS(CYB::Exception::Violation(static_cast<CYB::Exception::Violation::ErrorCode>(FakeExceptionCode)), CYB::Exception::Violation);
			THEN("The correct Violation exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_EXCEPTION_ERROR_CODE);
			}
		}
		WHEN("A SystemData instantiation is attempted") {
			REQUIRE_THROWS_AS(CYB::Exception::SystemData(static_cast<CYB::Exception::SystemData::ErrorCode>(FakeExceptionCode)), CYB::Exception::Violation);
			THEN("The correct Violation exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_EXCEPTION_ERROR_CODE);
			}
		}
		WHEN("A Internal instantiation is attempted") {
			REQUIRE_THROWS_AS(CYB::Exception::Internal(static_cast<CYB::Exception::Internal::ErrorCode>(FakeExceptionCode)), CYB::Exception::Violation);
			THEN("The correct Violation exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_EXCEPTION_ERROR_CODE);
			}
		}
		WHEN("A Fatal instantiation is attempted") {
			REQUIRE_THROWS_AS(CYB::Exception::Fatal(static_cast<CYB::Exception::Fatal::ErrorCode>(FakeExceptionCode)), CYB::Exception::Violation);
			THEN("The correct Violation exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_EXCEPTION_ERROR_CODE);
			}
		}
	}
}

TEST_CASE("REMOVE THIS AFTER YOU MAKE INTERNAL AND FATAL EXCEPTIONS", "[Exception][Unit]") {
	try {
		throw CYB::Exception::Internal(CYB::Exception::Internal::PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE);
	}
	catch (...) {}
	CHECK_EXCEPTION_CODE(CYB::Exception::Internal::PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE);
	try {
		throw CYB::Exception::Fatal(CYB::Exception::Fatal::PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE);
	}
	catch (...) {}
	CHECK_EXCEPTION_CODE(CYB::Exception::Fatal::PLEASE_REMOVE_ME_AS_SOON_AS_YOU_ADD_A_REAL_EXCEPTION_HERE);
}