#include "TestHeader.hpp"

class TestDependancies {
private:
	ModuleDependancy<CYB::Platform::Modules::Kernel32> FK32;
	ModuleDependancy<CYB::Platform::Modules::Shell> FShell;
	ModuleDependancy<CYB::Platform::Modules::ShellAPI> FShellAPI;
	ModuleDependancy<CYB::Platform::Modules::Ole32> FOLE;
	ModuleDependancy<CYB::Platform::Modules::LibC> FC;
	ModuleDependancy<CYB::Platform::Modules::PThread> FPThread;
};

SCENARIO("Logger construction works", "[Engine][Logger][Unit]") {
	GIVEN("The correct dependancies") {
		TestDependancies Deps;
		WHEN("A logger is constructed") {
			CYB::Platform::System::Console Emergency;
			CYB::Engine::Logger Log(Emergency);
			THEN("The log file exists") {
				CHECK(CYB::Platform::System::Path(Log.CurrentLog()).IsFile());
			}
		}
	}
}