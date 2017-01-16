#include "TestHeader.hpp"

class TestDependancies {
private:
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

SCENARIO("Logger logging works", "[Engine][Logger][Functional]") {
	using namespace CYB::API::String;
	TestDependancies Deps;
	CYB::Platform::System::Console Emergency;
	GIVEN("A logger") {
		CYB::Engine::Logger Log(Emergency);
		WHEN("Some logs are written to it") {
			Log.Log(Static(u8"A dev log"), CYB::API::Logger::Level::DEV);
			Log.Log(Static(u8"An info log"), CYB::API::Logger::Level::INFO);
			Log.Log(Static(u8"A warning log"), CYB::API::Logger::Level::WARN);
			Log.Log(Static(u8"An error log!"), CYB::API::Logger::Level::ERR);
#ifdef DEBUG
			REQUIRE_THROWS_AS(Log.Log(Static(u8"A bad enum log!"), static_cast<CYB::API::Logger::Level>(54)), CYB::Exception::Violation);
#endif
			Log.Flush();
			THEN("All is well") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
			}
		}
	}
}