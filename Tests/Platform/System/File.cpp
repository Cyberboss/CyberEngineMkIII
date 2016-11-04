#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

const char* const ALotOfData = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

class TestStartup {
private:
	std::unique_ptr<Path> TestPath1, TestPath2;
public:
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> FK32;
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> FShell;
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> FShellAPI;
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> FC;
public:
	TestStartup() :
		FK32(CYB::Core().FModuleManager.FK32),
		FShell(CYB::Core().FModuleManager.FShell),
		FShellAPI(CYB::Core().FModuleManager.FShellAPI),
		FC(CYB::Core().FModuleManager.FC)
	{
		//Clear the temp folder
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
		
		//Get some temp paths
		REQUIRE_NOTHROW(TestPath1.reset(new Path(Path::SystemPath::TEMPORARY)));
		REQUIRE_NOTHROW(TestPath1->Append(UTF8(Static("TestFile1")), false, false));
		REQUIRE_NOTHROW(TestPath2.reset(new Path(Path::SystemPath::TEMPORARY)));
		REQUIRE_NOTHROW(TestPath2->Append(UTF8(Static("TestFile2")), false, false));
	}
	Path Path1() {
		return *TestPath1;
	}
	Path Path2() {
		return *TestPath2;
	}
};

SCENARIO("Files can be touched", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A path to touch") {
		auto Path1(TestData.Path1());
		WHEN("It is touched") {
			File::Touch(Path(Path1));
			THEN("It now exists") {
				CHECK(Path1.IsFile());
			}
		}
	}
}

SCENARIO("Files sizes can be retrieved without opening them", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("File constructors work", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files sizes can be retrieved without opening them", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files can have their cursor position retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files can have their cursor position set set retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files can be read from", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files can be written to", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files' Paths can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files' OpenMethod can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}