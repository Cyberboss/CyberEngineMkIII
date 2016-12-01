#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

const char* const ALotOfData = "aabaaaaaaaaaaabbbaaaaaaabbbbaaaaaaaaaaaaaaabbbaaaaaaaaaaaaaaaabbaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

class TestStartup {
private:
	std::unique_ptr<Path> TestPath1, TestPath2, TestPathDir;
public:
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMKernel32> FK32;
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShell> FShell;
	ModuleDependancy<CYB::API::Platform::Identifier::WINDOWS, CYB::Platform::Modules::AMShellAPI> FShellAPI;
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> FC;
private:
	static void Write(unsigned long long AAmount, Path&& AFile, const File::Method AMethod) {
		File Data(std::move(AFile), File::Mode::WRITE, AMethod);
		while (AAmount > 0) {
			const auto DataWritten(Data.Write(ALotOfData, AAmount));
			REQUIRE(DataWritten <= AAmount);
			AAmount -= DataWritten;
		}
	}
public:
	TestStartup() {
		//Clear the temp folder
		REQUIRE_NOTHROW(Path(Path::SystemPath::TEMPORARY).Delete(true));
		
		//Get some temp paths
		REQUIRE_NOTHROW(TestPath1.reset(new Path(Path::SystemPath::TEMPORARY)));
		REQUIRE_NOTHROW(TestPath1->Append(UTF8(Static("TestFile1")), false, false));
		REQUIRE_NOTHROW(TestPath2.reset(new Path(Path::SystemPath::TEMPORARY)));
		REQUIRE_NOTHROW(TestPath2->Append(UTF8(Static("TestFile2")), false, false));
		REQUIRE_NOTHROW(TestPathDir.reset(new Path(Path::SystemPath::TEMPORARY)));
		REQUIRE_NOTHROW(TestPathDir->Append(UTF8(Static("Dir")), true, false));
	}
	void Data1(const unsigned long long AAmount, const File::Method AMethod = File::Method::ANY) const {
		Write(AAmount, Path1(), AMethod);
	}
	void Data2(const unsigned long long AAmount, const File::Method AMethod = File::Method::ANY) const {
		Write(AAmount, Path2(), AMethod);
	}
	void Touch1(void) const {
		REQUIRE_NOTHROW(File::Touch(Path(*TestPath1)));
	}
	void Touch2(void) const {
		REQUIRE_NOTHROW(File::Touch(Path(*TestPath2)));
	}
	Path Path1() const {
		return *TestPath1;
	}
	Path Path2() const {
		return *TestPath2;
	}
	Path Dir() const {
		return *TestPathDir;
	}
};

#ifdef TARGET_OS_WINDOWS
using namespace CYB::Platform::Win32;
REDIRECTED_FUNCTION(BadGetFileAttributesEx, const void* const, const CYB::Platform::Win32::GET_FILEEX_INFO_LEVELS, const void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadCreateFile, const void* const, const unsigned long, const unsigned long, const void* const, const unsigned long, const unsigned long, const void* const) {
	return INVALID_HANDLE_VALUE;
}

REDIRECTED_FUNCTION(BadSetFilePointerEx, const void* const, const LARGE_INTEGER, const void* const, const unsigned long) {
	return 0;
}

#endif
unsigned long long FakeStatReturn;
unsigned long long FakeStat2(Fake::SysCalls::Args&) {
	return FakeStatReturn;
}

unsigned long long FakeFStat(Fake::SysCalls::Args&) {
	return 1;
}

REDIRECTED_FUNCTION(BadLSeek, const unsigned long long, const unsigned long long, const unsigned long long) {
	return static_cast<unsigned long long>(-1);
}

REDIRECTED_FUNCTION(BadOpen, const unsigned long long, const void* const, const unsigned long long) {
	return -1;
}

REDIRECTED_FUNCTION(BadRead, const unsigned long long, const void* const, const unsigned long long) {
	return static_cast<unsigned long long>(-1);
}

REDIRECTED_FUNCTION(BadReadFile, const void* const, const void* const, const unsigned long, const void* const, const void* const) {
	return 0;
}


REDIRECTED_FUNCTION(BadWrite, const long long, const void* const, const unsigned long long) {
	return static_cast<unsigned long long>(-1);
}

REDIRECTED_FUNCTION(BadWriteFile, const void* const, const void* const, const unsigned long, const void* const, const void* const) {
	return 0;
}

REDIRECTED_FUNCTION(BadGetFileSizeEx, const void* const, const void* const) {
	return 0;
}

SCENARIO("Files can be touched", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A path to touch") {
		auto Path1(TestData.Path1());
		WHEN("It is touched") {
			CHECK_NOTHROW(File::Touch(Path(Path1)));
			THEN("It now exists") {
				CHECK(Path1.IsFile());
			}
		}
	}
}

SCENARIO("File constructors work", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	File::Mode Mo;
	File::Method Me;
	Path* CurrentPath;
	bool MovePath, Exists, Directory;

	const auto Creation([&]() {
		INFO("Opening with path:");
		INFO((*CurrentPath)());
		if (MovePath)
			File(std::move(*CurrentPath), Mo, Me);
		else
			File(*CurrentPath, Mo, Me);
	});

	const auto Then([&]() {
		if(!Directory)
			CHECK_NOTHROW(Creation());
		else {
			CHECK_THROWS_AS(Creation(), CYB::Exception::SystemData);
			CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_EXISTS);
		}
	});
	const auto MethodMatrix([&]() {
		WHEN("We open with method ANY") {
			Me = File::Method::ANY;
			Then();
		}
		WHEN("We open with method CREATE") {
			Me = File::Method::CREATE;
			if(!Exists)
				Then();
			else {
				THEN("It fails correctly") {
					CHECK_THROWS_AS(Creation(), CYB::Exception::SystemData);
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_EXISTS);
				}
			}
		}
		WHEN("We open with method TRUNCATE") {
			Me = File::Method::TRUNCATE;
			if(Mo != File::Mode::READ)
				Then();
			else {
				THEN("It fails correctly") {
#ifdef DEBUG
					CHECK_THROWS_AS(Creation(), CYB::Exception::Violation);
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_PARAMETERS);
#else
					CHECK(true);
#endif
				}
			}
		}
		WHEN("We open with method EXIST") {
			Me = File::Method::EXIST;
			if (Exists)
				Then();
			else {
				THEN("It fails correctly") {
					CHECK_THROWS_AS(Creation(), CYB::Exception::SystemData);
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_FOUND);
				}
			}
		}
	});

	const auto ModeMatrix([&]() {
		WHEN("We open with mode READ") {
			Mo = File::Mode::READ;
			MethodMatrix();
		}
		WHEN("We open with mode WRITE") {
			Mo = File::Mode::WRITE;
			MethodMatrix();
		}
		WHEN("We open with mode READ_WRITE") {
			Mo = File::Mode::READ_WRITE;
			MethodMatrix();
		}
	});
	const auto ConstructorMatrix([&]() {
		WHEN("We use the path move constructor") {
			MovePath = true;
			ModeMatrix();
		}
		WHEN("We use the path reference constructor") {
			MovePath = false;
			ModeMatrix();
		}
	});

	GIVEN("A non-existant path") {
		auto Test(TestData.Path1());
		CurrentPath = &Test;
		Exists = false;
		Directory = false;
		ConstructorMatrix();
	}
	GIVEN("A path that is a file") {
		auto Test(TestData.Path1());
		REQUIRE_NOTHROW(File::Touch(Path(Test)));
		CurrentPath = &Test;
		Exists = true;
		Directory = false;
		ConstructorMatrix();
	}
	GIVEN("A path that is a directory") {
		auto Test(TestData.Dir());
		REQUIRE_NOTHROW(Test.Append(UTF8(Static("SomeDir")), true, false));
		CurrentPath = &Test;
		Exists = true;
		Directory = true;
		ConstructorMatrix();
	}
	GIVEN("A path that is deleted") {
		auto Test(TestData.Dir());
		REQUIRE_NOTHROW(Test.Append(UTF8(Static("SomeDir")), true, false));
		REQUIRE_NOTHROW(Test.Delete(true));
		CurrentPath = &Test;
		Exists = false;
		Directory = false;
		ConstructorMatrix();
	}
	GIVEN("A valid file") {
		File DasFile(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY);
		WHEN("It is moved constructed") {
			File DasMove(std::move(DasFile));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("It is moved assigned") {
			File DasMove(TestData.Path2(), File::Mode::READ_WRITE, File::Method::ANY);
			REQUIRE_NOTHROW(DasMove = std::move(DasFile));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}

	//whitebox
	const auto Violation([&]() {
		WHEN("Creation is attempted") {
#ifdef DEBUG
			CHECK_THROWS_AS(File(TestData.Path1(), Mo, Me), CYB::Exception::Violation);
#endif
			THEN("The correct error is thrown") {
#ifdef DEBUG
				CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
#else
				CHECK(true);
#endif
			}
		}
	});
	GIVEN("A bad mode") {
		Mo = static_cast<File::Mode>(5);
		Me = File::Method::ANY;
		Violation();
	}
	GIVEN("A bad method") {
		Mo = File::Mode::READ_WRITE;
		Me = static_cast<File::Method>(5);
		Violation();
	}

	GIVEN("A bad open calls") {
#ifdef TARGET_OS_WINDOWS
		const auto BCF(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::CreateFileW, BadCreateFile>());
#else
		const auto BO(TestData.FC.Redirect<CYB::Platform::Modules::LibC::open, BadOpen>());
#endif
		WHEN("We give access errors") {
#ifdef TARGET_OS_WINDOWS
			const auto Thing(OverrideError(TestData.FK32, ERROR_ACCESS_DENIED));
#else
			errno = EACCES;
#endif
			WHEN("We try to open a file for reading") {
				CHECK_THROWS_AS(File(TestData.Path1(), File::Mode::READ, File::Method::EXIST), CYB::Exception::SystemData);
				THEN("The correct exception is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
				}
			}
			WHEN("We try to open a file for writing") {
				CHECK_THROWS_AS(File(TestData.Path1(), File::Mode::WRITE, File::Method::EXIST), CYB::Exception::SystemData);
				THEN("The correct exception is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
				}
			}
#ifdef TARGET_OS_WINDOWS
			GIVEN("Bad path checks") {
				const auto BPC(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::GetFileAttributesExW, BadGetFileAttributesEx>());
				WHEN("We try to open a file") {
					CHECK_THROWS_AS(File(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY), CYB::Exception::SystemData);
					THEN("The correct exception is thrown") {
						CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
					}
				}
			}
		}
		GIVEN("Coverage required on the error switch") {
			File::Mode Mo2(File::Mode::READ_WRITE);
			const auto CheckThis([&](const auto AError, const auto AException) {
				WHEN("We do this error") {
					const auto Thing2(OverrideError(TestData.FK32, static_cast<DWORD>(AError)));
					CHECK_THROWS_AS(File(TestData.Path1(), Mo2, File::Method::ANY), CYB::Exception::SystemData);
					THEN("The correct exception is thrown") {
						CHECK_EXCEPTION_CODE(AException);
					}
				}
			});
			CheckThis(ERROR_FILE_NOT_FOUND, CYB::Exception::SystemData::FILE_NOT_FOUND);
			CheckThis(ERROR_PATH_NOT_FOUND, CYB::Exception::SystemData::FILE_NOT_FOUND);
			CheckThis(ERROR_INVALID_NAME, CYB::Exception::SystemData::FILE_NOT_FOUND);
			CheckThis(ERROR_FILE_EXISTS, CYB::Exception::SystemData::FILE_EXISTS);
			CheckThis(ERROR_ACCESS_DENIED, CYB::Exception::SystemData::FILE_NOT_WRITABLE);
			CheckThis(ERROR_SHARING_VIOLATION, CYB::Exception::SystemData::FILE_NOT_WRITABLE);
			CheckThis(0, CYB::Exception::SystemData::FILE_NOT_WRITABLE);
			Mo2 = File::Mode::READ;
			CheckThis(ERROR_ACCESS_DENIED, CYB::Exception::SystemData::FILE_NOT_READABLE);
			CheckThis(ERROR_SHARING_VIOLATION, CYB::Exception::SystemData::FILE_NOT_READABLE);
			CheckThis(0, CYB::Exception::SystemData::FILE_NOT_READABLE);
#endif
		}
	}
}

SCENARIO("Files sizes can be retrieved after opening them", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A file with some data") {
		TestData.Data1(20);
		WHEN("It is opened") {
			File TF(TestData.Path1(), File::Mode::READ, File::Method::EXIST);
			THEN("It has the same size") {
				CHECK(TF.Size() == 20U);
			}
			AND_WHEN("The sizing call fails") {
#ifndef TARGET_OS_WINDOWS
				SysCallOverride BS(CYB::Platform::System::Sys::FSTAT, FakeFStat);
#endif
				const auto Thing(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::GetFileSizeEx, BadGetFileSizeEx>());
				THEN("The correct error is thrown") {
					CHECK_THROWS_AS(TF.Size(), CYB::Exception::SystemData);
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
				}
			}
		}
	}
}

SCENARIO("Files can have their cursor position set and retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A file with some data") {
		TestData.Data1(10);
		WHEN("It is opened") {
			File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::EXIST);
			AND_THEN("It's cursor position is retrieved") {
				const auto Result(TF.CursorPosition());
				THEN("It is at the beginning") {
					REQUIRE(Result == 0U);
					AND_WHEN("It is set using Seek(0, Cur)") {
						const auto Result2(TF.Seek(0, File::SeekLocation::CURSOR));
						THEN("It is the same") {
							CHECK(Result == Result2);
						}
					}
				}
			}
			AND_THEN("It's cursor position is set to the end") {
				const auto Result(TF.Seek(0, File::SeekLocation::END));
				THEN("It is at the end") {
					REQUIRE(Result == 10U);
					AND_WHEN("It is seeked back to the beginning") {
						const auto Result2(TF.Seek(0, File::SeekLocation::BEGIN));
						THEN("It is at the beginning") {
							CHECK(Result2 == 0U);
						}
					}
				}
			}
			AND_THEN("It's cursor position is set to some offset of the beginning") {
				const auto Result(TF.Seek(5, File::SeekLocation::BEGIN));
				THEN("It is there") {
					CHECK(Result == 5U);
				}
			}
			AND_THEN("It's cursor position is set to some offset of the cursor") {
				const auto Result(TF.Seek(5, File::SeekLocation::CURSOR));
				THEN("It is there") {
					CHECK(Result == 5U);
				}
			}
			AND_THEN("It's cursor position is set to some offset of the end") {
				const auto Result(TF.Seek(-5, File::SeekLocation::END));
				THEN("It is there") {
					CHECK(Result == 5U);
				}
			}
			AND_WHEN("We seek up a bit") {
				REQUIRE(TF.Seek(1, File::SeekLocation::CURSOR));
				AND_THEN("Some stuff is read") {
					char Dest[5];
					REQUIRE_NOTHROW(TF.Read(Dest, 5));
					THEN("It has moved") {
						CHECK(TF.CursorPosition() == 6U);
					}
				}
				AND_THEN("Some stuff is writted") {
					REQUIRE_NOTHROW(TF.Write(ALotOfData, 5));
					THEN("It has moved") {
						CHECK(TF.CursorPosition() == 6U);
					}
				}
			}
			AND_WHEN("We seek invalidly") {
#ifdef DEBUG
				REQUIRE_THROWS_AS(TF.Seek(1, static_cast<File::SeekLocation>(5)), CYB::Exception::Violation);
#endif
				THEN("The correct error is thrown") {
#ifdef DEBUG
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_ENUM);
#else
					CHECK(true);
#endif
				}
			}
			AND_WHEN("The seek failed") {
#ifdef TARGET_OS_WINDOWS
				const auto Thing1(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::SetFilePointerEx, BadSetFilePointerEx>());
#else
				const auto Thing2(TestData.FC.Redirect<CYB::Platform::Modules::LibC::lseek, BadLSeek>());
#endif
				REQUIRE_THROWS_AS(TF.Seek(-5, File::SeekLocation::END), CYB::Exception::SystemData);
				THEN("The correct error is thrown") {
					CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
				}
			}
		}
	}
}

SCENARIO("Files can be read from", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	File::Mode Mo;
	File::Method Me;
	bool HasData, Fail(false);
	const auto OpenAndRead([&]() {
		if (Mo == File::Mode::READ && Me == File::Method::TRUNCATE) {
			THEN("We won't run this check as it'll fail on opening") {
				CHECK(true);
			}
		}
		else {
			AND_THEN("It is opened") {
				File TF(TestData.Path1(), Mo, Me);
				AND_THEN("It is read from") {
					char Data[21];
					if (Mo != File::Mode::WRITE) {
						const auto Result(TF.Read(Data, 21));
						THEN("The correct amount of bytes were read (No good OS should fail this check) and the data is correct") {
							const auto Expected((HasData && !Fail) ? 21U : 0);
							CHECK(Result == Expected);
							if (HasData && !Fail) {
								CYB::API::String::Dynamic TS1(CYB::API::String::Static(Data), 21),
									TS2(CYB::API::String::Static(ALotOfData), 21);
								CHECK(TS1 == TS2);
							}
						}
					}
					else {
#ifdef DEBUG
						CHECK_THROWS_AS(TF.Read(Data, 21), CYB::Exception::Violation);
#endif
						THEN("The correct exception is thrown") {
#ifdef DEBUG
							CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_OPERATION);
#else
							CHECK(true);
#endif
						}
					}
				}
			}
		}
	});

	const auto MethodMatrix([&]() {
		WHEN("It is opened with method ANY") {
			Me = File::Method::ANY;
			OpenAndRead();
		}
		WHEN("It is opened with method EXIST") {
			Me = File::Method::EXIST;
			OpenAndRead();
		}
		WHEN("It is opened with method TRUNCATE") {
			Me = File::Method::TRUNCATE;
			HasData = false;
			OpenAndRead();
		}
	});

	const auto PermissionsMatrix([&]() {
		WHEN("It is opened with Mode::READ") {
			Mo = File::Mode::READ;
			MethodMatrix();
		}
		WHEN("It is opened with Mode::READ") {
			Mo = File::Mode::WRITE;
			MethodMatrix();
		}
		WHEN("It is opened with Mode::READ_WRITE") {
			Mo = File::Mode::READ_WRITE;
			MethodMatrix();
		}
	});

	const auto FailMatrix([&]() {
		PermissionsMatrix();
		WHEN("The calls fails") {
			auto Thing1(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::ReadFile, BadReadFile>());
			auto Thing2(TestData.FC.Redirect<CYB::Platform::Modules::LibC::read, BadRead>());
			Fail = true;
			PermissionsMatrix();
		};
	});

	GIVEN("A file with some data") {
		TestData.Data1(21);
		HasData = true;
		FailMatrix();
	}
	GIVEN("A file with no data") {
		File::Touch(TestData.Path1());
		HasData = false;
		FailMatrix();
	}
}

SCENARIO("Files can be written to", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	File::Mode Mo;
	File::Method Me;
	bool HasData, Fail(false);
	const auto OpenAndWrite([&]() {
		if (Mo == File::Mode::READ && Me == File::Method::TRUNCATE) {
			THEN("We won't run this check as it'll fail on opening") {
				CHECK(true);
			}
		}
		else {
			AND_THEN("It is opened") {
				File TF(TestData.Path1(), Mo, Me);
				AND_THEN("We try to write to it") {
					if (Mo != File::Mode::READ) {
						const auto Result(TF.Write(ALotOfData, 21));
						THEN("The correct amount of bytes were written (No good OS should fail this check)") {
							const auto Expected(Fail ? 0U : 21U);
							CHECK(Result == Expected);
						}
					}
					else {
#ifdef DEBUG
						CHECK_THROWS_AS(TF.Write(ALotOfData, 21), CYB::Exception::Violation);
#endif
						THEN("The correct exception is thrown") {
#ifdef DEBUG
							CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_OPERATION);
#else
							CHECK(true);
#endif
						}
					}
				}
			}
		}
	});

	const auto MethodMatrix([&]() {
		WHEN("It is opened with method ANY") {
			Me = File::Method::ANY;
			OpenAndWrite();
		}
		WHEN("It is opened with method EXIST") {
			Me = File::Method::EXIST;
			OpenAndWrite();
		}
		WHEN("It is opened with method TRUNCATE") {
			Me = File::Method::TRUNCATE;
			HasData = false;
			OpenAndWrite();
		}
	});

	const auto PermissionsMatrix([&]() {
		WHEN("It is opened with Mode::READ") {
			Mo = File::Mode::READ;
			MethodMatrix();
		}
		WHEN("It is opened with Mode::READ") {
			Mo = File::Mode::WRITE;
			MethodMatrix();
		}
		WHEN("It is opened with Mode::READ_WRITE") {
			Mo = File::Mode::READ_WRITE;
			MethodMatrix();
		}
	});

	const auto FailMatrix([&]() {
		PermissionsMatrix();
		WHEN("The call fails") {
			auto Thing1(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::WriteFile, BadWriteFile>());
			auto Thing2(TestData.FC.Redirect<CYB::Platform::Modules::LibC::write, BadWrite>());
			Fail = true;
			PermissionsMatrix();
		}
	});

	GIVEN("A file with some data") {
		TestData.Data1(21);
		HasData = true;
		FailMatrix();
	}
	GIVEN("A file with no data") {
		File::Touch(TestData.Path1());
		HasData = false;
		FailMatrix();
	}
}

SCENARIO("Files sizes can be retrieved without opening them", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A file with some data") {
		TestData.Data1(5);
		WHEN("It's size is statically checked") {
			const auto Result(File::Size(TestData.Path1()));
			THEN("It is correct") {
				CHECK(Result == 5U);
			}
		}
		GIVEN("Bad stat calls") {
			const auto DoCheck([&](const CYB::Exception::SystemData::ErrorCode AError) {
				WHEN("It's size is statically checked") {
					CHECK_THROWS_AS(File::Size(TestData.Path1()), CYB::Exception::SystemData);
					THEN("The correct error is thrown") {
						CHECK_EXCEPTION_CODE(AError);
					}
				}
			});
#ifdef TARGET_OS_WINDOWS
			const auto BPC(TestData.FK32.Redirect<CYB::Platform::Modules::Kernel32::GetFileAttributesExW, BadGetFileAttributesEx>());
			WHEN("The error is set to ACCESS_DENIED") {
				const auto Thing(OverrideError(TestData.FK32, ERROR_ACCESS_DENIED));
				DoCheck(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
			WHEN("The error is set to SHARING_VIOLATION") {
				const auto Thing(OverrideError(TestData.FK32, ERROR_SHARING_VIOLATION));
				DoCheck(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
			WHEN("The error is set to anything other than SHARING_VIOLATION and ACCESS_DENIED") {
				const auto Thing(OverrideError(TestData.FK32, 0));
				DoCheck(CYB::Exception::SystemData::FILE_NOT_FOUND);
			}
#else
			SysCallOverride BS(Sys::CallNumber::LSTAT, FakeStat2);
			WHEN("The error is set to EACCES") {
				FakeStatReturn = EACCES;
				DoCheck(CYB::Exception::SystemData::FILE_NOT_READABLE);
			}
			WHEN("The error is set to anything other that EACCES") {
				FakeStatReturn = EAGAIN;
				DoCheck(CYB::Exception::SystemData::FILE_NOT_FOUND);
			}
#endif
		}
	}
	GIVEN("A file with no data") {
		File::Touch(TestData.Path1());
		WHEN("It's size is statically checked") {
			const auto Result(File::Size(TestData.Path1()));
			THEN("It is correct") {
				CHECK(Result == 0U);
			}
		}
	}
	GIVEN("An empty Path") {
		auto Path(TestData.Path1());
		WHEN("It's size is statically checked") {
			CHECK_THROWS_AS(File::Size(std::move(Path)), CYB::Exception::SystemData);
			THEN("The correct exception is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_FOUND);
			}
		}
	}
}

SCENARIO("Files' Paths can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A file") {
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY);
		WHEN("It's path is retrieved") {
			auto& FilePath(TF.GetPath());
			THEN("It is the same as when it was opened") {
				CHECK(FilePath() == TestData.Path1()());
			}
		}
	}
}

SCENARIO("Files' OpenMethod can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A freshly created file opened with Method::CREATE") {
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::CREATE);
		WHEN("The open method is checked") {
			const auto Result(TF.OpenMethod());
			THEN("It is Method::CREATE") {
				CHECK(Result == File::Method::CREATE);
			}
		}
	}
	GIVEN("A freshly created file opened with Method::ANY"){
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY);
		WHEN("The open method is checked") {
			const auto Result(TF.OpenMethod());
			THEN("It is Method::CREATE") {
				CHECK(Result == File::Method::CREATE);
			}
		}
	}
	GIVEN("A pre-existing file opened with Method::ANY") {
		File::Touch(TestData.Path1());
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY);
		WHEN("The open method is checked") {
			const auto Result(TF.OpenMethod());
			THEN("It is Method::EXIST") {
				CHECK(Result == File::Method::EXIST);
			}
		}
	}
	GIVEN("A pre-existing file opened with Method::EXIST") {
		File::Touch(TestData.Path1());
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::EXIST);
		WHEN("The open method is checked") {
			const auto Result(TF.OpenMethod());
			THEN("It is Method::EXIST") {
				CHECK(Result == File::Method::EXIST);
			}
		}
	}
	GIVEN("A pre-existing file with data opened with Method::TRUNCATE") {
		TestData.Data1(10);
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::TRUNCATE);
		WHEN("The open method is checked") {
			const auto Result(TF.OpenMethod());
			THEN("It is Method::TRUNCATE") {
				CHECK(Result == File::Method::TRUNCATE);
			}
		}
		WHEN("The data size is checked") {
			const auto Size(TF.Size());
			THEN("It is zero") {
				CHECK(Size == 0U);
			}
		}
	}
}

SCENARIO("Files' OpenMode can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A pre-existing file opened with Mode::READ") {
		TestData.Data1(5);
		File TF(TestData.Path1(), File::Mode::READ, File::Method::ANY);
		WHEN("The open mode is checked") {
			const auto Result(TF.OpenMode());
			THEN("It is Mode::READ") {
				CHECK(Result == File::Mode::READ);
			}
		}
	}
	GIVEN("A pre-existing file opened with Mode::WRITE") {
		TestData.Data1(5);
		File TF(TestData.Path1(), File::Mode::WRITE, File::Method::ANY);
		WHEN("The open mode is checked") {
			const auto Result(TF.OpenMode());
			THEN("It is Mode::WRITE") {
				CHECK(Result == File::Mode::WRITE);
			}
		}
	}
	GIVEN("A pre-existing file opened with Mode::READ_WRITE") {
		TestData.Data1(5);
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY);
		WHEN("The open mode is checked") {
			const auto Result(TF.OpenMode());
			THEN("It is Mode::READ_WRITE") {
				CHECK(Result == File::Mode::READ_WRITE);
			}
		}
	}
}
