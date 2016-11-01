#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

SCENARIO("File touching the unimplemented functions for code coverage", "[Platform][System][File][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);

	Path Test(Path::SystemPath::TEMPORARY);
	Test.Append(UTF8(Static("HelloWorld")), false, false);
	File From(Test, File::Mode::READ_WRITE, File::Method::ANY);
	File To(std::move(From));
	Path Test2(Path::SystemPath::TEMPORARY);
	Test2.Append(UTF8(Static("HelloWorld2")), false, false);
	File TheFile(std::move(Test2), File::Mode::READ_WRITE, File::Method::ANY);
	TheFile.Size();
	TheFile.CursorPosition();
	TheFile.Seek(0, File::SeekLocation::CURSOR);
	TheFile.Read(nullptr, 0);
	TheFile.Write(nullptr, 0);
	TheFile.Path();
}