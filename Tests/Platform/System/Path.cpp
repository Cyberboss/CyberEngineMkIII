#include "TestHeader.hpp"

using namespace CYB::Platform::System;

SCENARIO("Paths can be created by the system", "[Platform][System][Path][Unit") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("That paths can be constructed with a system directory") {
		Path* TestPath(nullptr);
		WHEN("The executable directory is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemDirectory::EXECUTABLE));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
			delete TestPath;
		}
	}
}