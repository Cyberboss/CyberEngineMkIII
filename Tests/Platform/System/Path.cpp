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
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);

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
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);

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
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);

	GIVEN("Some setup files") {
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
		Path Setup1(Path::SystemPath::TEMPORARY), Setup2(Setup1), Setup3(Setup1);
		REQUIRE_NOTHROW(Setup1.Append(UTF8(Static(u8"TestDir")), true, false));
		REQUIRE_NOTHROW(Setup2.Append(UTF8(Static(u8"TestFile")), false, false));
		Touch(Setup2);
		REQUIRE_NOTHROW(Setup3.Append(UTF8(Static(u8"FakeFile")), false, false));

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
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);
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
			REQUIRE_NOTHROW(Result1 = Subject.FullName());
			REQUIRE_NOTHROW(Result2 = Subject.Name());
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
			REQUIRE_NOTHROW(Result1 = Subject.FullName());
			REQUIRE_NOTHROW(Result2 = Subject.Name());
			REQUIRE_NOTHROW(Result3 = Subject.Extension());
			THEN("The correct results are given") {
				CHECK(Result1 == Static(u8"Te.st.File2"));
				CHECK(Result2 == Static(u8"Te.st"));
				CHECK(Result3 == Static(u8"File2"));
			}
		}
	}

}

SCENARIO("Path ascending works", "[Platform][System][Path][Unit]") {

	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);

	REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));

	GIVEN("A valid Path") {
		Path TestPath(Path::SystemPath::TEMPORARY);
		const auto Prior(TestPath());
		WHEN("The path is ascended from a file") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static("Doesn'tExist")), false, false));
			REQUIRE_NOTHROW(TestPath.NavigateToParentDirectory());
			THEN("It succeeds") {
				CHECK(Prior == TestPath());
			}
		}
		WHEN("The path is ascended from a directory") {
			REQUIRE_NOTHROW(TestPath.Append(UTF8(Static("SomeDir")), true, false));
			REQUIRE_NOTHROW(TestPath.NavigateToParentDirectory());
			THEN("It succeeds") {
				CHECK(Prior == TestPath());
			}
		}
		WHEN("Ascention is attempted past the root directory") {
			const auto Lambda([&]() {
				for (auto I(0U); I < 100; ++I)
					TestPath.NavigateToParentDirectory();
			});
			REQUIRE_THROWS_AS(Lambda(), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
		}
	}
}

SCENARIO("Path directory enumeration works", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);
	REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
	GIVEN("A path with some entries") {
		Path TestPath(Path::SystemPath::TEMPORARY), File(TestPath), Folder(TestPath), FolderWithFiles(TestPath);
		REQUIRE_NOTHROW(File.Append(UTF8(Static(u8"SomeFile")), false, false));
		Touch(File);
		REQUIRE_NOTHROW(Folder.Append(UTF8(Static(u8"SomeFolder")), true, false));
		REQUIRE_NOTHROW(FolderWithFiles.Append(UTF8(Static(u8"SomeFolderWithFiles")), true, false));
		REQUIRE_NOTHROW(FolderWithFiles.Append(UTF8(Static(u8"SomeFile")), false, false));
		Touch(FolderWithFiles);
		WHEN("The path's contents are enumerated") {
			auto FoundCount(0U);
			const auto Lambda([&]() {
				auto Entry(TestPath.Contents());
				for (auto& It(Entry()); It->Valid(); ++It) {
					if ((*It)().Name() == Static(u8"SomeFile"))
						++FoundCount;
					else if ((*It)().Name() == Static(u8"SomeFolder"))
						++FoundCount;
					else if ((*It)().Name() == Static(u8"SomeFolderWithFiles"))
						++FoundCount;
				}
			});
			REQUIRE_NOTHROW(Lambda());
			THEN("Everything is found") {
				CHECK(FoundCount == 3);
			}
		}
		WHEN("The empty path's contents are enumerated") {
			bool Result(true);
			REQUIRE_NOTHROW(Result = Folder.Contents()()->Valid());
			THEN("Nothing is found") {
				CHECK_FALSE(Result);
			}
		}
	}
}

template <class ARedirector> class BadCreateDirectory;
template <class ARedirector> class BadRealPath;
template <class ARedirector> class BadDeleteFile;
int DeleteFileSucceedsIf1(0);
template <class ARedirector> class BadPathFileExists;
template <class ARedirector> class BadRmDir;
template <class ARedirector> class BadUnlink;
template <class ARedirector> class BadSetFileAttributes;
template <class ARedirector> class GoodSetFileAttributes;
template <class ARedirector> class BadPathRemoveFileSpec;
unsigned long long FakeStat(Fake::SysCalls::Args&);

SCENARIO("Path whitebox", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMOle32> OLE(CYB::Core().FModuleManager.FOLE);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);
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
	GIVEN("A valid file") {
		Path Setup(Path::SystemPath::TEMPORARY);
		REQUIRE_NOTHROW(Setup.Append(UTF8(Static(u8"TestFile")), false, false));
		Touch(Setup);
		WHEN("The deletion functions are messed with") {
			const auto BDF(K32.Redirect<CYB::Platform::Modules::Kernel32::DeleteFileW, BadDeleteFile>());
			const auto BUL(LibC.Redirect<CYB::Platform::Modules::LibC::unlink, BadUnlink>());
			AND_WHEN("The error is File not found") {
#ifdef TARGET_OS_WINDOWS
				const auto Error(OverrideError(K32, ERROR_FILE_NOT_FOUND));
#else
				const auto Error(OverrideError(K32, ENOENT));
#endif
				THEN("The function call succeeds") {
					CHECK_NOTHROW(Setup.Delete(false));
				}
			}
#ifdef TARGET_OS_WINDOWS
			AND_WHEN("The error is a read only type") {
				const auto Error(OverrideError(K32, ERROR_ACCESS_DENIED));
				AND_WHEN("Setting the readonly property fails") {
					const auto BSFA(K32.Redirect<CYB::Platform::Modules::Kernel32::SetFileAttributesW, BadSetFileAttributes>());
					REQUIRE_THROWS_AS(Setup.Delete(false), CYB::Exception::SystemData);
					THEN("The function call fails") {
						CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
					}
				}
				AND_WHEN("Deleting the file fails again") {
					const auto GSFA(K32.Redirect<CYB::Platform::Modules::Kernel32::SetFileAttributesW, GoodSetFileAttributes>());
					REQUIRE_THROWS_AS(Setup.Delete(false), CYB::Exception::SystemData);
					THEN("The function call fails") {
						CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
					}
				}
				AND_WHEN("Deleting the file succeeds afterwards again") {
					DeleteFileSucceedsIf1 = 2;
					THEN("The function call succeeds") {
						CHECK_NOTHROW(Setup.Delete(false));
					}
				}
			}
#endif
			AND_WHEN("The error is unknown") {
				const auto Error(OverrideError(K32, 0));
				REQUIRE_THROWS_AS(Setup.Delete(false), CYB::Exception::SystemData);
				THEN("The function call fails") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
				}
			}
		}
	}
	GIVEN("A valid directory") {
		Path Setup(Path::SystemPath::TEMPORARY);
		REQUIRE_NOTHROW(Setup.Append(UTF8(Static("SomeDir")), true, false));
		WHEN("A deletion is attempted but the directory is 'removed' prior to it's completion") {
			const auto BRMD(LibC.Redirect<CYB::Platform::Modules::LibC::rmdir, BadRmDir>());
			const auto BRD(K32.Redirect<CYB::Platform::Modules::Kernel32::RemoveDirectoryW, BadPathRemoveFileSpec>());
#ifdef TARGET_OS_WINDOWS
			const auto Error(OverrideError(K32, ERROR_FILE_NOT_FOUND));
#else
			const auto Error(OverrideError(K32, ENOENT));
#endif
			REQUIRE_NOTHROW(Setup.Delete(false));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("A deletion is attempted but fails for sommmmeeee reason") {
			const auto BRMD(LibC.Redirect<CYB::Platform::Modules::LibC::rmdir, BadRmDir>());
			const auto BRD(K32.Redirect<CYB::Platform::Modules::Kernel32::RemoveDirectoryW, BadPathRemoveFileSpec>());
			const auto Error(OverrideError(K32, 0));
			REQUIRE_THROWS_AS(Setup.Delete(false), CYB::Exception::SystemData);
			THEN("All is well") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
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
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);
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

REDIRECTED_FUNCTION(BadSetFileAttributes, const void* const, const unsigned int) {
	return 0;
}

REDIRECTED_FUNCTION(GoodSetFileAttributes, const void* const, const unsigned int) {
	return 1;
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

REDIRECTED_FUNCTION(BadRmDir, const void* const) {
	return -1;
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

REDIRECTED_FUNCTION(BadNSGetPath, const void* const, const unsigned long long) {
	return -1;
}

REDIRECTED_FUNCTION(BadDeleteFile, const void* const) {
	if (DeleteFileSucceedsIf1 == 2) {
		--DeleteFileSucceedsIf1;
		return 0;
	}
	else if (DeleteFileSucceedsIf1 == 0) {
		return 0;
	}
	else {
		--DeleteFileSucceedsIf1;
		return 1;
	}
}

REDIRECTED_FUNCTION(BadUnlink, const void* const) {
	return -1;
}

SCENARIO("Path errors work", "[Platform][System][Path][Unit]") {
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> ShellAPI(CYB::Core().FModuleManager.FShellAPI);
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> Shell(CYB::Core().FModuleManager.FShell);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	ModuleDependancy<CYB::API::Platform::OSX, CYB::Platform::Modules::AMDyLD> DyLD(CYB::Core().FModuleManager.FDyLD);
	GIVEN("Faulty system path retrival") {
		{
			const auto BRL(LibC.Redirect<CYB::Platform::Modules::LibC::readlink, BadReadLink>());
			const auto BNSGP(DyLD.Redirect<CYB::Platform::Modules::DyLD::_NSGetExecutablePath, BadNSGetPath>());
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