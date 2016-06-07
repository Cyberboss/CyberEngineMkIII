#include "TestHeader.hpp"

SCENARIO("Basic Mutex functions work", "[Platform][System][Mutex][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMPThread> PThread(CYB::Core().FModuleManager.FPThread);
	GIVEN("An empty Mutex pointer") {
		CYB::Platform::System::Mutex* TestMutex(nullptr);
		WHEN("The mutex is initialized") {
			TestMutex = new CYB::Platform::System::Mutex();
			THEN("All is well with the world") {
				CHECK_COOL_AND_CALM;
			}
			delete TestMutex;
		}
	}
}