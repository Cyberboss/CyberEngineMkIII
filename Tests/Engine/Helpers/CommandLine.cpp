#include "TestHeader.hpp"

template <> void CYB::Engine::Helpers::CommandLine::Backdoor<>(CommandLine& ACL) {
	REQUIRE(ACL.FTokens.size() == 6U);
	THEN("Index 0 is short") {
		CHECK(ACL.FTokens[0].FEntry == API::String::Static(u8"s"));
		CHECK(ACL.FTokens[0].FType == TokenType::SINGLE_LETTER_KEY);
	}
	THEN("Index 1 is normal") {
		CHECK(ACL.FTokens[1].FEntry == API::String::Static(u8"Hello world"));
		CHECK(ACL.FTokens[1].FType == TokenType::NORMAL);
	}
	THEN("Index 2 is normal") {
		CHECK(ACL.FTokens[2].FEntry == API::String::Static(u8"How are you"));
		CHECK(ACL.FTokens[2].FType == TokenType::NORMAL);
	}
	THEN("Index 3 is normal") {
		CHECK(ACL.FTokens[3].FEntry == API::String::Static(u8"asdf"));
		CHECK(ACL.FTokens[3].FType == TokenType::NORMAL);
	}
	THEN("Index 4 is short") {
		CHECK(ACL.FTokens[4].FEntry == API::String::Static(u8"ad"));
		CHECK(ACL.FTokens[4].FType == TokenType::SINGLE_LETTER_KEY);
	}
	THEN("Index 5 is extended") {
		CHECK(ACL.FTokens[5].FEntry == API::String::Static(u8"extended"));
		CHECK(ACL.FTokens[5].FType == TokenType::EXTENDED_KEY);
	}
}

SCENARIO("CommandLine parsing works", "[Engine][Helpers][CommandLine][Unit]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> FK32;
	GIVEN("A command line") {
		const char* Command[7]{ u8"/s", u8"Hello world", u8"How are you", u8"asdf", u8"-ad", nullptr, u8"--extended" };
		const wchar_t* CommandWide[7]{ L"/s", L"Hello world", L"How are you", L"asdf", L"-ad", nullptr, L"--extended" };

		CYB::Engine::Helpers::CommandLine CL(7,
#ifdef TARGET_OS_WINDOWS
			CommandWide
#else
			Command
#endif
		);
		WHEN("We check on the tokens") {
			CYB::Engine::Helpers::CommandLine::Backdoor(CL);
		}
	}
}
static bool FCommandLineExpectedRun, FFakeExit2Ran;
static int RunCount;
static unsigned long long FakeProcExit2(Fake::SysCalls::Args&) {
	FFakeExit2Ran = true;
	return 1;
}

SCENARIO("CommandLine callbacks work", "[Engine][Helpers][CommandLine][Functional]") {
	ModuleDependancy<CYB::Platform::Modules::Kernel32> FK32;
	GIVEN("A command line") {
		const char* Command[7]{ u8"/s", u8"Hello world", u8"How are you", u8"asdf", u8"-ad", nullptr, u8"--extended" };
		const wchar_t* CommandWide[7]{ L"/s", L"Hello world", L"How are you", L"asdf", L"-ad", nullptr, L"--extended" };

		CYB::Engine::Helpers::CommandLine CL(7,
#ifdef TARGET_OS_WINDOWS
			CommandWide
#else
			Command
#endif
		);
		AND_THEN("We run some handlers") {
			FCommandLineExpectedRun = false;
			FFakeExit2Ran = false;
			SysCallOverride Exit(
#ifdef TARGET_OS_WINDOWS
				CYB::Platform::System::Sys::TERMINATE_PROC
#else
				CYB::Platform::System::Sys::EXIT_PROC
#endif
				, FakeProcExit2);
			WHEN("We test short invocation for '/'") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) {
					FCommandLineExpectedRun = true;
					return true;
				}, 0, 0, CYB::API::String::Static(u8"s"), CYB::API::String::Static(u8"ajksdfhjk"), 0, 0, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test short invocation for '-'") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) {
					FCommandLineExpectedRun = true;
					return true;
			}, 0, 0, CYB::API::String::Static(u8"ad"), CYB::API::String::Static(u8"ajksdfhjk"), 0, 0, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test long invocation") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) {
					FCommandLineExpectedRun = true;
					return true;
				}, 0, 0, CYB::API::String::Static(u8"aasdfased"), CYB::API::String::Static(u8"extended"), 0, 0, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test required arguments") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>& thing) {
					FCommandLineExpectedRun = thing.size() == 1 && *thing[0] == CYB::API::String::Static(u8"Hello world");
					return true;
				}, 0, 0, CYB::API::String::Static(u8"s"), CYB::API::String::Static(u8"sdfgsdfgs"), 1, 0, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test lack of required arguments") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) {
					FCommandLineExpectedRun = true;
					return true;
				}, 0, 0, CYB::API::String::Static(u8"aasdfased"), CYB::API::String::Static(u8"extended"), 1, 0, 1);
				THEN("It didn't run") {
					CHECK_FALSE(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test optional arguments") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>& thing) {
					FCommandLineExpectedRun = thing.size() == 1 && *thing[0] == CYB::API::String::Static(u8"Hello world");
					return true;
				}, 0, 0, CYB::API::String::Static(u8"s"), CYB::API::String::Static(u8"sdfgsdfgs"), 0, 1, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test lack of optional arguments") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>& thing) {
					FCommandLineExpectedRun = thing.size() == 0;
					return true;
				}, 0, 0, CYB::API::String::Static(u8"asdfasdfasds"), CYB::API::String::Static(u8"extended"), 0, 1, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test required and optional arguments") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>& thing) {
					FCommandLineExpectedRun = thing.size() == 2 && *thing[0] == CYB::API::String::Static(u8"Hello world") && *thing[1] == CYB::API::String::Static(u8"How are you");
					return true;
				}, 0, 0, CYB::API::String::Static(u8"s"), CYB::API::String::Static(u8"sdfgsdfgs"), 1, 1, 1);
				THEN("It ran") {
					CHECK(FCommandLineExpectedRun);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			RunCount = 0;
			WHEN("We test maximum invocations") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) {
					++RunCount;
					return true;
				}, 0, 0, CYB::API::String::Static(u8"s"), CYB::API::String::Static(u8"extended"), 0, 0, 1);
				THEN("It ran ONCE") {
					CHECK(RunCount == 1);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test maximum invocations in a slightly different way") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) {
					++RunCount;
					return true;
				}, 0, 0, CYB::API::String::Static(u8"s"), CYB::API::String::Static(u8"extended"), 0, 0, 2);
				THEN("It ran twice") {
					CHECK(RunCount == 2);
					CHECK_FALSE(FFakeExit2Ran);
				}
			}
			WHEN("We test return exiting") {
				CL.RunHandler([](const CYB::API::Container::Deque<const CYB::API::String::Dynamic*>&) { return false; }, 0, 0, CYB::API::String::Static(u8"ad"), CYB::API::String::Static(u8"ajksdfhjk"), 0, 0, 1);
				THEN("It tried to exit") {
					CHECK(FFakeExit2Ran);
				}
			}
		}
	}
}