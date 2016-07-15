#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

static void Touch(const Path& APath);
SCENARIO("Paths can be deleted", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);

	GIVEN("Some existing items") {
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
		{
			Path Setup(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Setup.Append(UTF8(Static(u8"RecursivePath/Recurse/Recurse")), true, true));
			REQUIRE_NOTHROW(Setup.Append(UTF8(Static(u8"SomeFile")), false, false));
			Touch(Setup);
		}
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Append(UTF8(Static(u8"TestPath")), true, false));
		{
			Path Setup(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Setup.Append(UTF8(Static(u8"TestPath2")), true, false));
			REQUIRE_NOTHROW(Setup.Append(UTF8(Static(u8"SomeFile")), false, false));
			Touch(Setup);
		}
		WHEN("An empty folder is deleted non-recursively") {
			Path Test(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Test.Append(UTF8(Static(u8"TestPath")), false, false));
			REQUIRE_NOTHROW(Test.Delete(false));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("An empty folder is deleted recursively") {
			Path Test(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Test.Append(UTF8(Static(u8"TestPath")), false, false));
			REQUIRE_NOTHROW(Test.Delete(true));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("A file is deleted non-recursively") {
			Path Test(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Test.Append(UTF8(Static(u8"TestPath2/SomeFile")), false, false));
			REQUIRE_NOTHROW(Test.Delete(false));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("A file is deleted recursively") {
			Path Test(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Test.Append(UTF8(Static(u8"TestPath2/SomeFile")), false, false));
			REQUIRE_NOTHROW(Test.Delete(true));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("An non-empty folder is deleted non-recursively") {
			Path Test(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Test.Append(UTF8(Static(u8"RecursivePath")), false, false));
			REQUIRE_THROWS_AS(Test.Delete(false), CYB::Exception::SystemData);
			THEN("The appropriate error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::DIRECTORY_NOT_EMPTY);
			}
		}
		WHEN("An non-empty folder is deleted recursively") {
			Path Test(Path::SystemPath::TEMPORARY);
			REQUIRE_NOTHROW(Test.Append(UTF8(Static(u8"RecursivePath")), false, false));
			REQUIRE_NOTHROW(Test.Delete(true));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Path Append works", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);

	REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
	REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Append(UTF8(Static(u8"ExistingPath/Recurse/Recurse")), true, true));

	GIVEN("A valid Path") {
		Path TestPath(Path::SystemPath::TEMPORARY);
		WHEN("Something too long is appended onto it") {
			REQUIRE_THROWS_AS(TestPath.Append(UTF8(Static(u8"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")), false, false), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::PATH_TOO_LONG);
			}
		}
		WHEN("A file is appended onto it that doesn't exist") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static(u8"TestPath")), false, false));
			THEN("It will have succeeded") {
				CHECK(true);
			}
		}
		WHEN("A folder is appended onto it that doesn't exist") {
			REQUIRE_THROWS_AS(TestPath.Append(UTF8(Static(u8"TestPath/Recurse")), false, false), CYB::Exception::SystemData);
			THEN("It will have failed") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
		}
		WHEN("A recursive folder is appended onto it that doesn't exist and is created, but not recursively") {
			REQUIRE_THROWS_AS(TestPath.Append(UTF8(Static(u8"TestPath2/Recurse/Recurse")), true, false), CYB::Exception::SystemData);
			THEN("It will have failed") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
		}
		WHEN("A folder is appended onto it that does exist") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static(u8"ExistingPath/Recurse")), false, false));
			THEN("It will have succeeded") {
				CHECK(true);
			}
		}
		WHEN("A recursive folder is appended onto it that does exist") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static(u8"ExistingPath/Recurse/Recurse")), false, false));
			THEN("It will have succeeded") {
				CHECK(true);
			}
		}
		WHEN("A folder is appended onto it that doesn't exist and is created") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static(u8"TestPath3")), true, false));
			THEN("It will have succeeded") {
				CHECK(true);
			}
		}
		WHEN("A recursive folder is appended onto it that doesn't exist and is created recursively") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static(u8"TestPath2/Recurse")), true, true));
			THEN("It will have succeeded") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Path Directory Seperator works", "[Platform][System][Path][Unit]") {
	CHECK(Path::DirectorySeparatorChar() == Static(u8"/"));
}

SCENARIO("Path file type identification works", "[Platform][System][Path][Unit]") {

	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);

	GIVEN("Some setup files") {
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
		Path Setup1(Path::SystemPath::TEMPORARY), Setup2(Setup1), Setup3(Setup1);
		Setup1.Append(UTF8(Static(u8"TestDir")), true, false);
		Setup2.Append(UTF8(Static(u8"TestFile")), false, false);
		Touch(Setup2);
		Setup3.Append(UTF8(Static(u8"FakeFile")), false, false);

		WHEN("A file is asked if it is a file") {
			bool Result(false);
			REQUIRE_NOTHROW(Result = Setup2.IsFile());
			THEN("The result is correct") {
				CHECK(Result);
			}
		}
		WHEN("A file is asked if it is a directory") {
			bool Result(true);
			REQUIRE_NOTHROW(Result = Setup2.IsDirectory());
			THEN("The result is correct") {
				CHECK_FALSE(Result);
			}
		}
		WHEN("A directory is asked if it is a file") {
			bool Result(true);
			REQUIRE_NOTHROW(Result = Setup1.IsFile());
			THEN("The result is correct") {
				CHECK_FALSE(Result);
			}
		}
		WHEN("A directory is asked if it is a directory") {
			bool Result(false);
			REQUIRE_NOTHROW(Result = Setup1.IsDirectory());
			THEN("The result is correct") {
				CHECK(Result);
			}
		}
		WHEN("A nothing is asked if it is a file") {
			bool Result(false);
			REQUIRE_THROWS_AS(Result = Setup3.IsFile(), CYB::Exception::SystemData);
			THEN("The result is correct") {
				CHECK_FALSE(Result);
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::PATH_LOST);
			}
		}
		WHEN("A nothing is asked if it is a directory") {
			bool Result(false);
			REQUIRE_THROWS_AS(Result = Setup3.IsDirectory(), CYB::Exception::SystemData);
			THEN("The result is correct") {
				CHECK_FALSE(Result);
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::PATH_LOST);
			}
		}

	}

}

SCENARIO("Path file name parsing works", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("Some variously named files"){
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
		Path Setup1(Path::SystemPath::TEMPORARY), Setup2(Setup1), Setup3(Setup1), Setup4(Setup1);
		Setup1.Append(UTF8(Static(u8"TestFile1")), true, false);
		Setup2.Append(UTF8(Static(u8"Te.st.File2")), false, false);
		Touch(Setup1);
		Touch(Setup2);
		WHEN("A file without an extension is checked") {
			auto& Subject(Setup1);
			UTF8 Result1, Result2, Result3;
			REQUIRE_NOTHROW(Result1 = Subject.FullFileName());
			REQUIRE_NOTHROW(Result2 = Subject.FileName());
			REQUIRE_NOTHROW(Result3 = Subject.Extension());
			THEN("The correct results are given") {
				CHECK(Result1 == Static(u8"TestFile1"));
				CHECK(Result2 == Static(u8"TestFile1"));
				CHECK(Result3 == Static(u8"TestFile1"));
			}
		}
		WHEN("A file with an extension is checked") {
			auto& Subject(Setup2);
			UTF8 Result1, Result2, Result3;
			REQUIRE_NOTHROW(Result1 = Subject.FullFileName());
			REQUIRE_NOTHROW(Result2 = Subject.FileName());
			REQUIRE_NOTHROW(Result3 = Subject.Extension());
			THEN("The correct results are given") {
				CHECK(Result1 == Static(u8"Te.st.File2"));
				CHECK(Result2 == Static(u8"Te.st"));
				CHECK(Result3 == Static(u8"File2"));
			}
		}
	}

}

template <class ARedirector> class BadCreateDirectory;
template <class ARedirector> class BadRealPath;
template <class ARedirector> class BadPathFileExists;
unsigned long long FakeStat(Fake::SysCalls::Args&);

SCENARIO("Path whitebox", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	{
		Path Setup(Path::SystemPath::TEMPORARY);
		REQUIRE_NOTHROW(Setup.Delete(true));
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Append(UTF8(Static(u8"ExistingPath/Recurse/Recurse")), true, true));
	}
	GIVEN("A valid path") {
		Path TestPath(Path::SystemPath::TEMPORARY);
		WHEN("We whitebox Append") {
			WHEN("We do a basic cd Append") {
				const UTF8 Appendage(Static(u8"ExistingPath"));
				AND_THEN("The evaluation fails") {
					const auto BPC(ShellAPI.Redirect<CYB::Platform::Modules::ShellAPI::PathCanonicalizeW, BadCreateDirectory>());
					const auto BRP(LibC.Redirect<CYB::Platform::Modules::LibC::realpath, BadRealPath>());
					REQUIRE_THROWS_AS(TestPath.Append(Appendage, false, false), CYB::Exception::SystemData);
					THEN("It fails") {
						CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
					}
				}
				AND_THEN("The pre-verification fails") {
					const auto BPFE(ShellAPI.Redirect<CYB::Platform::Modules::ShellAPI::PathFileExistsW, BadPathFileExists>());
#ifndef TARGET_OS_WINDOWS
					SysCallOverride BS(Sys::CallNumber::LSTAT, FakeStat);
#endif
					REQUIRE_THROWS_AS(TestPath.Append(Appendage, false, false), CYB::Exception::SystemData);
					THEN("The correct exception is thrown") {
						CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::PATH_LOST);
					}
				}
			}
		}
	}
	try {
		Path(Path::SystemPath::TEMPORARY).Delete(true);
	}
	catch (...) {}
}






//TODO: Use File::Touch after it's implemented
static void Touch(const Path& APath) {
#ifdef TARGET_OS_WINDOWS
	using namespace CYB::Platform::Win32;
	const Path Myself(Path::SystemPath::EXECUTABLE_IMAGE);
	UTF16 Version(Myself()), OtherVersion(APath());
	CopyFileW(Version.WString(), OtherVersion.WString(), FALSE);
#else
	using namespace CYB::Platform::Posix;
	auto Command(UTF8(Static(u8"touch ")) + APath());

	system(Command.CString());
#endif
}






// old stuff

unsigned long long FakeStat(Fake::SysCalls::Args&) {
	return 1;
}

REDIRECTED_FUNCTION(BadPathFileExists, const void* const) {
	return 0;
}

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
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A test Path") {
		const Path TestPath(Path::SystemPath::TEMPORARY);
		WHEN("The string is retrieved") {
			auto Cop(TestPath());
			THEN("All is well") {
				CHECK(Cop.RawLength() > 0);
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
#ifdef DEBUG
		WHEN("An invalid enum is used") {
			REQUIRE_THROWS_AS(Path((Path::SystemPath) - 1), CYB::Exception::Violation);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
			}
		}
#endif
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