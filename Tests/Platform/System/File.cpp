#include "TestHeader.hpp"

using namespace CYB::Platform::System;
using namespace CYB::API::String;

const char* const ALotOfData = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

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
		std::unique_ptr<File> DasFile;
		REQUIRE_NOTHROW(DasFile.reset(new File(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY)));
		WHEN("It is moved constructed") {
			std::unique_ptr<File> DasMove;
			REQUIRE_NOTHROW(DasMove.reset(new File(std::move(*DasFile))));
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("It is moved assigned") {
			std::unique_ptr<File> DasMove;
			REQUIRE_NOTHROW(DasMove.reset(new File(TestData.Path2(), File::Mode::READ_WRITE, File::Method::ANY)));
			REQUIRE_NOTHROW(*DasMove = std::move(*DasFile));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
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
		}
	}
}

SCENARIO("Files can have their cursor position retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files can have their cursor position set and retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files can be read from", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	File::Mode Mo;
	File::Method Me;
	bool HasData;
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
						THEN("The read data is correct") {
							CHECK(Result == (HasData ? 21U : 0));
							if (HasData) {
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
	GIVEN("A file with some data") {
		TestData.Data1(21);
		HasData = true;
		PermissionsMatrix();
	}
	GIVEN("A file with no data") {
		File::Touch(TestData.Path1());
		HasData = false;
		PermissionsMatrix();
	}
}

SCENARIO("Files can be written to", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}

SCENARIO("Files sizes can be retrieved without opening them", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A touched file") {
		TestData.Touch1();
		WHEN("It's size is checked") {
			unsigned long long Size(1);
			CHECK_NOTHROW(Size = File::Size(TestData.Path1()));
			THEN("It is zero") {
				CHECK(Size == 0U);
			}
		}
	}
}

SCENARIO("Files' Paths can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	GIVEN("A file") {
		File TF(TestData.Path1(), File::Mode::READ_WRITE, File::Method::ANY);
		WHEN("It's path is retrieved") {
			auto& FilePath(TF.Path());
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