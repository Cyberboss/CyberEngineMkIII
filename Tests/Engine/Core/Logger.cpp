#include "TestHeader.hpp"

class TestDependancies {
public:
	ModuleDependancy<CYB::Platform::Modules::Kernel32> FK32;
	ModuleDependancy<CYB::Platform::Modules::Shell> FShell;
	ModuleDependancy<CYB::Platform::Modules::ShellAPI> FShellAPI;
	ModuleDependancy<CYB::Platform::Modules::Ole32> FOLE;
	ModuleDependancy<CYB::Platform::Modules::LibC> FC;
	ModuleDependancy<CYB::Platform::Modules::PThread> FPThread;
public:
	~TestDependancies() {
		//loggers fuck with the context
		Fake::Core::ResetToFakeCorePointer();
	}
};

SCENARIO("Logger construction works", "[Engine][Logger][Functional]") {
	GIVEN("The correct dependancies") {
		TestDependancies Deps;
		CYB::Platform::System::Console Emergency;
		WHEN("A logger is constructed") {
			CYB::Engine::Logger Log(Emergency);
			THEN("The log file exists") {
				CHECK(CYB::Platform::System::Path(Log.CurrentLog()).IsFile());
			}
		}
	}
}

#ifdef TARGET_OS_WINDOWS
REDIRECTED_FUNCTION(BadCreateFile, const void* const, const unsigned long, const unsigned long, const void* const, const unsigned long, const unsigned long, const void* const) {
	using namespace CYB::Platform::Win32;
	return INVALID_HANDLE_VALUE;
}
#endif

template <> void CYB::Engine::Logger::Backdoor<int>(int& AIgnored) {
	static_cast<void>(AIgnored);
#if defined(DEBUG) && defined(TARGET_OS_WINDOWS)
	static_cast<Logger*>(nullptr)->SelfAsThreadable();
#endif
}

SCENARIO("Logger logging works", "[Engine][Logger][Functional]") {
	using namespace CYB::API::String;
	TestDependancies Deps;
	CYB::Platform::System::Console Emergency;
	GIVEN("A logger") {
		CYB::Engine::Logger Log(Emergency);
		WHEN("Some logs are written to it") {
#ifdef DEBUG
			REQUIRE_THROWS_AS(Log.Log(Static(u8"A bad enum log!"), static_cast<CYB::API::Logger::Level>(54)), CYB::Exception::Violation);
#endif

#ifdef TARGET_OS_WINDOWS
			//do some scheduling trickery to make sure we hit the code coverage line in Flush
			class ThreadPriority {
			public:
				ThreadPriority() {
					CYB::Platform::Win32::SetThreadPriority(CYB::Platform::Win32::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
				}
				~ThreadPriority() {
					CYB::Platform::Win32::SetThreadPriority(CYB::Platform::Win32::GetCurrentThread(), THREAD_PRIORITY_NORMAL);
				}
			};
			ThreadPriority GiveItToMe;
#endif

			Log.Log(Static(u8"A dev log"), CYB::API::Logger::Level::DEV);
			Log.Log(Static(u8"An info log"), CYB::API::Logger::Level::INFO);
			Log.Log(Static(u8"A warning log"), CYB::API::Logger::Level::WARN);
			Log.Log(Static(u8"An error log!"), CYB::API::Logger::Level::ERR);
			Log.Flush();
			THEN("All is well") {
#ifdef DEBUG
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
#else
				CHECK(true);
#endif
			}
		}
		WHEN("We try to open another") {
			CYB::Engine::Logger Log2(static_cast<CYB::API::Logger&>(Log));
			THEN("It delays for a bit, but works") {
				const auto Kajigger([&]() { Log2.Log(Static(u8"Hoi!"), CYB::API::Logger::Level::INFO); });
				CHECK_NOTHROW(Kajigger());
			}
		}
		WHEN("We check the current log") {
			UTF8 Result(Log.CurrentLog());
			AND_THEN("Convert it to a path") {
				CYB::Platform::System::Path ThePath(std::move(Result));
				THEN("It is valid") {
					CHECK(ThePath.IsFile());
				}
			}
		}
		WHEN("The things I do for code coverage") {
			int bluh;
			CHECK_NOTHROW(CYB::Engine::Logger::Backdoor(bluh));
		}
	}
	GIVEN("A bad file opener") {
		const auto BCF(Deps.FK32.Redirect<CYB::Platform::Modules::Kernel32::CreateFileW, BadCreateFile>());
#ifdef TARGET_OS_WINDOWS
		const auto Thing(OverrideError(Deps.FK32, ERROR_ACCESS_DENIED));
#else
		errno = EACCES;
#endif
		WHEN("We try to open the log") {
			REQUIRE_THROWS_AS(CYB::Engine::Logger(static_cast<CYB::API::Logger&>(Emergency)), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::STREAM_NOT_WRITABLE);
			}
		}
	}
}