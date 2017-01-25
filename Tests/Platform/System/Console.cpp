#include "TestHeader.hpp"


SCENARIO("Console logging works", "[Platform][System][Console]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> K32;
	GIVEN("A console and message") {
		CYB::Platform::System::Console Con;
		CYB::API::String::Static Msg(u8"Hello world");
		WHEN("We log debug") {
			Con.SetDebugLogging(false);
			REQUIRE_NOTHROW(Con.Log(Msg, CYB::API::Logger::Level::DEV));
			Con.SetDebugLogging(true);
			REQUIRE_NOTHROW(Con.Log(Msg, CYB::API::Logger::Level::DEV));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We log info") {
			REQUIRE_NOTHROW(Con.Log(Msg, CYB::API::Logger::Level::INFO));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We log warning") {
			REQUIRE_NOTHROW(Con.Log(Msg, CYB::API::Logger::Level::WARN));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We log error") {
			REQUIRE_NOTHROW(Con.Log(Msg, CYB::API::Logger::Level::ERR));
			THEN("All is well") {
				CHECK(true);
			}
		}
#ifdef DEBUG
		WHEN("We log invalidly") {
			REQUIRE_THROWS_AS(Con.Log(Msg, static_cast<CYB::API::Logger::Level>(42)), CYB::Exception::Violation);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
			}
		}
#endif
		WHEN("We flush") {
			REQUIRE_NOTHROW(Con.Flush());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We get the current log") {
			REQUIRE_NOTHROW(Con.CurrentLog());
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("We show it") {
			REQUIRE_NOTHROW(Con.Show());
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}