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

REDIRECTED_FUNCTION(BadCreateFile, const void* const, const unsigned long, const unsigned long, const void* const, const unsigned long, const unsigned long, const void* const) {
#ifdef TARGET_OS_WINDOWS
	using namespace CYB::Platform::Win32;
	return INVALID_HANDLE_VALUE;
#endif
}
REDIRECTED_FUNCTION(BadWrite, const long long, const void* const, const unsigned long long) {
	return static_cast<unsigned long long>(-1);
}

REDIRECTED_FUNCTION(BadWriteFile, const void* const, const void* const, const unsigned long, const void* const, const void* const) {
	return 0;
}


template <> void CYB::Engine::Logger::Backdoor<int>(int& AIgnored) {
	static_cast<void>(AIgnored);
#if defined(DEBUG) && defined(TARGET_OS_WINDOWS)
	static_cast<Logger*>(nullptr)->SelfAsThreadable();
#endif
}

//do some scheduling trickery to make sure we hit the code coverage line in Flush
class ThreadPriority {
public:
#ifdef TARGET_OS_WINDOWS
	ThreadPriority() {
		CYB::Platform::Win32::SetThreadPriority(CYB::Platform::Win32::GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	}
	~ThreadPriority() {
		CYB::Platform::Win32::SetThreadPriority(CYB::Platform::Win32::GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	}
#endif
};

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

			ThreadPriority GiveItToMe;

			Log.Log(Static(u8"A dev log"), CYB::API::Logger::Level::DEV);
			Log.Log(Static(u8"An info log"), CYB::API::Logger::Level::INFO);
			Log.Log(Static(u8"A warning log"), CYB::API::Logger::Level::WARN);
			Log.Log(Static(u8"An error log!"), CYB::API::Logger::Level::ERR);
			AND_THEN("Oh no, we can't write to the log file anymore!") {
				const auto BWF(Deps.FK32.Redirect<CYB::Platform::Modules::Kernel32::WriteFile, BadWriteFile>());
				const auto BW(Deps.FC.Redirect<CYB::Platform::Modules::LibC::write, BadWrite>());
				Log.Flush();
				THEN("It terminates and logs to the console") {
#ifdef DEBUG
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
#else
					CHECK(true);
#endif
				}
			}
			AND_THEN("Things proceed normally") {
				Log.Flush();
				THEN("All is well") {
#ifdef DEBUG
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
#else
					CHECK(true);
#endif
				}
			}
		}
		WHEN("We try immediately to open another") {
			CYB::Engine::Logger Log2(static_cast<CYB::API::Logger&>(Log));
			THEN("It delays for a bit, but works") {
				const auto Kajigger([&]() { Log2.Log(Static(u8"Hoi!"), CYB::API::Logger::Level::INFO); });
				CHECK_NOTHROW(Kajigger());
			}
		}
		WHEN("We mess with debug logging") {
			Log.SetDebugLogging(false);
			Log.Log(Static(u8"A dev log"), CYB::API::Logger::Level::DEV);
			Log.SetDebugLogging(true);
			Log.Log(Static(u8"Another dev log"), CYB::API::Logger::Level::DEV);
			THEN("All is well") {
				CHECK(true);
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
		WHEN("We intentionally overload the logger") {
			ThreadPriority GiveItToMe;
			Fake::FailedAllocationCount = 5;
			const auto ToDo([&]() {
				Log.Log(Static(u8"No, Mr. Logger, I expect you to die"), CYB::API::Logger::Level::INFO);
				THEN("All is well") {
					CHECK(true);
				}
			});

			AND_THEN("Oh no, we can't write to the log file anymore!") {
				const auto BWF(Deps.FK32.Redirect<CYB::Platform::Modules::Kernel32::WriteFile, BadWriteFile>());
				const auto BW(Deps.FC.Redirect<CYB::Platform::Modules::LibC::write, BadWrite>());
				ToDo();
			}
			AND_THEN("Things proceed normally") {
				ToDo();
			}

			Fake::FailedAllocationCount = 0;
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