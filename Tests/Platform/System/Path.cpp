#include "TestHeader.hpp"
#include "../Engine/Platform/CYBUTF16String.inl"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

bool SpecialReforkCase(true);
SCENARIO("Paths can be created by the system", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("That paths can be constructed with a system directory") {
		Path* TestPath(nullptr);
		WHEN("The executable directory is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemPath::EXECUTABLE));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
		}
		WHEN("The executable image path is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemPath::EXECUTABLE_IMAGE));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
		}
		WHEN("The resource directory is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemPath::RESOURCE));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
		}
		WHEN("The temporary directory is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemPath::TEMPORARY));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
		}
		WHEN("The user directory is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemPath::USER));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
		}
		WHEN("The working directory is retrieved") {
			REQUIRE_NOTHROW(TestPath = new Path(Path::SystemPath::WORKING));
			THEN("All is well") {
				CHECK(TestPath != nullptr);
			}
		}
		delete TestPath;
	}
}

SCENARIO("Path string retrieval operator works", "[Platform][System][Path][Unit]") {
	GIVEN("A UTF8 string interpreted as a Path") {
		UTF8 Fake(Static(u8"asdf"));
		const Path& Ref(*reinterpret_cast<Path*>(&Fake));
		WHEN("The string is retrieved") {
			auto Cop(Ref());
			THEN("They are equal") {
				CHECK(Cop == Fake);
			}
		}
	}
}

REDIRECTED_FUNCTION(BadPathRemoveFileSpec, void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadGetModuleHandle, void* const) {
	return static_cast<void*>(nullptr);
}

REDIRECTED_FUNCTION(BadReadLink, const void* const, const void* const, const long long) {
	return 0;
}

REDIRECTED_FUNCTION(BadGetTempPath, const unsigned long, const void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadCreateDirectory, const void* const, const void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadMkDir, const void* const, const unsigned long long) {
	return -1;
}

unsigned long LastError;

REDIRECTED_FUNCTION(BadGetLastError) {
	return LastError;
}

SCENARIO("Path errors work", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("Faulty system path retrival") {
		{
			const auto BRL(LibC.Redirect<CYB::Platform::Modules::LibC::readlink, BadReadLink>());
			WHEN("The executable path is corrupted") {
				const auto BPRFS(ShellAPI.Redirect<CYB::Platform::Modules::ShellAPI::PathRemoveFileSpecW, BadPathRemoveFileSpec>());
				REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::EXECUTABLE), CYB::Exception::SystemData);
				THEN("The correct error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
			}
			WHEN("The executable image path is corrupted") {
				const auto BGMH(K32.Redirect<CYB::Platform::Modules::Kernel32::GetModuleHandleW, BadGetModuleHandle>());
				REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::EXECUTABLE), CYB::Exception::SystemData);
				THEN("The correct error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
			}
		}
#ifdef TARGET_OS_WINDOWS
		WHEN("The temp path is corrupted") {
			const auto BGTP(K32.Redirect<CYB::Platform::Modules::Kernel32::GetTempPathW, BadGetTempPath>());
			REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::TEMPORARY), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
			}
		}
#endif
		{
			const auto BCD(K32.Redirect<CYB::Platform::Modules::Kernel32::CreateDirectoryW, BadCreateDirectory>());
			const auto BGLE(K32.Redirect<CYB::Platform::Modules::Kernel32::GetLastError, BadGetLastError>());
			const auto BMD(LibC.Redirect<CYB::Platform::Modules::LibC::mkdir, BadMkDir>());
			WHEN("The temp path directory creation fails") {
				errno = -1;
				LastError = 0;
				REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::TEMPORARY), CYB::Exception::SystemData);
				THEN("The correct error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
			}
			WHEN("The temp path directory creation fails due to prior existance") {
				errno = EEXIST;
				LastError = 183L;	//ERROR_ALREADY_EXISTS
				REQUIRE_NOTHROW(Path TestPath(Path::SystemPath::TEMPORARY));
				THEN("All is well") {
					CHECK(true);
				}
			}
		}
		WHEN("An invalid enum is used") {
			REQUIRE_THROWS_AS(Path((Path::SystemPath) - 1), CYB::Exception::Violation);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
			}
		}
	}
}
SCENARIO("FUCK YOU", "[Platform][System][Path][Unit]"){
	REQUIRE_THROWS_AS(Path((Path::SystemPath) - 1), CYB::Exception::Violation);
	THEN("The correct error is thrown") {
		CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
	}
}