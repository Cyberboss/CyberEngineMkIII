#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

const char* const ALotOfData = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

class NeededModules {
public:
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> FK32;
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> FShell;
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> FShellAPI;
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> FC;
public:
	NeededModules() :
		FK32(CYB::Core().FModuleManager.FK32),
		FShell(CYB::Core().FModuleManager.FShell),
		FShellAPI(CYB::Core().FModuleManager.FShellAPI),
		FC(CYB::Core().FModuleManager.FC)
	{}
};

SCENARIO("Files can be touched", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files sizes can be retrieved without opening them", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("File constructors work", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files sizes can be retrieved without opening them", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files can have their cursor position retrieved", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files can have their cursor position set set retrieved", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files can be read from", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files can be written to", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files' Paths can be retrieved", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}

SCENARIO("Files' OpenMethod can be retrieved", "[Platform][System][File][Unit]") {
	NeededModules TestModules;
	FAIL("Unwritten test");
}