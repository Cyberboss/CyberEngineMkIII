#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

bool SpecialReforkCase(true);
REDIRECTED_FUNCTION(GoodCreateDirectory, const void* const, const void* const) {
	return 1;
}
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
			//for code coverage purposes, fake the directory creation
			const auto GCD(K32.Redirect<CYB::Platform::Modules::Kernel32::CreateDirectoryW, GoodCreateDirectory>());
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

REDIRECTED_FUNCTION(BadGetCurrentDirectory, const unsigned long, const void* const) {
	return 0;
}

#ifdef TARGET_OS_WINDOWS
REDIRECTED_FUNCTION(BadGetAppData, const CYB::Platform::Win32::KNOWNFOLDERID&, const unsigned long, const void* const, const void* const) {
	return 1;
}
#endif

REDIRECTED_FUNCTION(BadRealPath, const void* const, const void* const) {
	return static_cast<char*>(nullptr);
}

REDIRECTED_FUNCTION(BadGetPwuid, const long, const void* const, const void* const, const long, const void* const) {
	return -1;
}

const char* env, *nextenv;
REDIRECTED_FUNCTION(RedirectedGetEnv, const void* const AEnv) {
	auto Result(env);
	env = nextenv;
	return Result;
}

REDIRECTED_FUNCTION(BadSysConf, const int) {
	return -1L;
}

SCENARIO("Path errors work", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
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
		WHEN("The resource path fails to evaluate") {
			const auto BPC(ShellAPI.Redirect<CYB::Platform::Modules::ShellAPI::PathCanonicalizeW, BadCreateDirectory>());
			const auto BRP(LibC.Redirect<CYB::Platform::Modules::LibC::realpath, BadRealPath>());
			REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::RESOURCE), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
			}
		}
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
		WHEN("The working directory is corrupted") {
			const auto BGCD(K32.Redirect<CYB::Platform::Modules::Kernel32::GetCurrentDirectoryW, BadGetCurrentDirectory>());
			const auto BRP(LibC.Redirect<CYB::Platform::Modules::LibC::realpath, BadRealPath>());
			REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::WORKING), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
			}
		}
#ifdef TARGET_OS_WINDOWS
		WHEN("The user directory is corrupted") {
			const auto BSHGKFP(Shell.Redirect<CYB::Platform::Modules::Shell::SHGetKnownFolderPath, BadGetAppData>());
			REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::USER), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
			}
		}
#else
		//have to fail this like 6 different ways
		WHEN("The working directory is retrieved") {
			const auto RGE(LibC.Redirect<CYB::Platform::Modules::LibC::getenv, RedirectedGetEnv>());
			env = nullptr;
			nextenv = nullptr;
			AND_WHEN("The xdg dir is set") {
				env = u8"asdf";
				REQUIRE_NOTHROW(Path TestPath(Path::SystemPath::USER));
				THEN("All is well") {
					CHECK(true);
				}
			}
			AND_WHEN("The home dir is set") {
				nextenv = u8"asdf";
				REQUIRE_NOTHROW(Path TestPath(Path::SystemPath::USER));
				THEN("All is well") {
					CHECK(true);
				}
			}
			AND_WHEN("The sysconf call fails") {
				const auto BSC(LibC.Redirect<CYB::Platform::Modules::LibC::sysconf, BadSysConf>());
				REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::USER), CYB::Exception::SystemData);
				THEN("The correct error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
			}
			AND_WHEN("The getpwuid_r call fails") {
				const auto BGPWUID(LibC.Redirect<CYB::Platform::Modules::LibC::getpwuid_r, BadGetPwuid>());
				REQUIRE_THROWS_AS(Path TestPath(Path::SystemPath::USER), CYB::Exception::SystemData);
				THEN("The correct error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE);
				}
			}
			AND_WHEN("The getpwuid_r call succeeds") {
				REQUIRE_NOTHROW(Path TestPath(Path::SystemPath::USER));
				THEN("All is well") {
					CHECK(true);
				}
			}
		}
#endif
	}
}