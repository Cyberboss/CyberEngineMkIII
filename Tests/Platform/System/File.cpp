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
private:
	void Write(unsigned long long AAmount, Path&& AFile, const File::Method AMethod) {
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
	}
	void Data1(const unsigned long long AAmount, const File::Method AMethod = File::Method::ANY) {
		Write(AAmount, Path1(), AMethod);
	}
	void Data2(const unsigned long long AAmount, const File::Method AMethod = File::Method::ANY) {
		Write(AAmount, Path2(), AMethod);
	}
	void Touch1(void) {
		REQUIRE_NOTHROW(File::Touch(Path(*TestPath1)));
	}
	void Touch2(void) {
		REQUIRE_NOTHROW(File::Touch(Path(*TestPath2)));
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
		File(MovePath ? std::move(*CurrentPath) : *CurrentPath, Mo, Me);
	});

	const auto Then([&]() {
		if(!Directory)
			CHECK_NOTHROW(Creation());
		else {
			CHECK_THROWS_AS(Creation(), CYB::Exception::SystemData);
			if(Mo == File::Mode::READ)
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_READABLE);
			else
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::FILE_NOT_WRITABLE);
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
					CHECK_THROWS_AS(Creation(), CYB::Exception::Violation);
					CHECK_EXCEPTION_CODE(CYB::Exception::Violation::INVALID_PARAMETERS);
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
		auto Test(TestData.Path1());
		REQUIRE_NOTHROW(Test.Append(UTF8(Static("SomeDir")), true, false));
		REQUIRE_NOTHROW(File::Touch(Path(Test)));
		CurrentPath = &Test;
		Exists = true;
		Directory = true;
		ConstructorMatrix();
	}
	GIVEN("A path that is deleted") {
		auto Test(TestData.Path1());
		REQUIRE_NOTHROW(Test.Append(UTF8(Static("SomeDir")), true, false));
		REQUIRE_NOTHROW(Test.Delete(true));
		REQUIRE_NOTHROW(File::Touch(Path(Test)));
		CurrentPath = &Test;
		Exists = true;
		Directory = false;
		ConstructorMatrix();
	}
}

SCENARIO("Files sizes can be retrieved after opening them", "[Platform][System][File][Unit]") {
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
	FAIL("Unwritten test");
}

SCENARIO("Files' OpenMethod can be retrieved", "[Platform][System][File][Unit]") {
	TestStartup TestData;
	FAIL("Unwritten test");
}