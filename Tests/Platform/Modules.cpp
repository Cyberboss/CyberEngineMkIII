#include "TestHeader.hpp"

static const CYB::API::String::Static ExistingLibrary(
#ifdef TARGET_OS_WINDOWS
	u8"Kernel32"
#else
	LIBDL_SO
#endif
);
static const CYB::API::String::Static ExistingLibraryFunctions[2]{
#ifdef TARGET_OS_WINDOWS
	u8"LoadLibraryA",
	u8"GetProcAddress"
#else
	u8"dlopen",
	u8"dlsym"
#endif
};
static const CYB::API::String::Static FakeLibrary("Thisreallyshouldntexistimeancomeon");
static const CYB::API::String::Static FakeLibraryFunctions[2]{
	u8"Yourmother",
	u8"THEHULLHASBEENBREACHEDANDTHESCIENCEISLEAKINGOOUUUUTT!!!"
};

SCENARIO("Modules can load a library", "[Platform][Modules][Functional]") {
	GIVEN("An existing library") {
		auto& Library(ExistingLibrary);
		WHEN("The library name is used to construct a Module") {
			CYB::Platform::Modules::Module* Result(nullptr);
			REQUIRE_NOTHROW(Result = new CYB::Platform::Modules::Module(Library));
			THEN("It is loaded successfully") {
				CHECK_COOL_AND_CALM;
				REQUIRE(Result != nullptr);
			}
			delete Result;
		}
	};
	GIVEN("A non existant library") {
		auto& Library(FakeLibrary);
		WHEN("The library name is used to construct a Module") {
			CYB::Platform::Modules::Module* Result(nullptr);
			CHECK_THROWS_AS(Result = new CYB::Platform::Modules::Module(Library), CYB::Exception::SystemData);
			THEN("It fails to load") {
				CHECK_COOL_AND_CALM;
				CHECK(Result == nullptr);
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MODULE_LOAD_FAILURE);
			}
			delete Result;
		}
	}
};

SCENARIO("Functions can be loaded from modules", "[Platform][Modules][Functional]") {
	GIVEN("A loaded library") {
		CYB::Platform::Modules::Module Mod(ExistingLibrary);
		WHEN("Legitimate functions are loaded from the library"){
			void* F1(nullptr),* F2(nullptr);
			CHECK_NOTHROW(F1 = Mod.LoadFunction(ExistingLibraryFunctions[0]));
			CHECK_NOTHROW(F2 = Mod.LoadFunction(ExistingLibraryFunctions[1]));
			THEN("They are not null and valid code locations") {
				CHECK_COOL_AND_CALM;
				CHECK(F1 != nullptr);
				CHECK(F2 != nullptr);
#ifdef TARGET_OS_WINDOWS
				CHECK(CYB::Platform::Win32::IsBadCodePtr(static_cast<CYB::Platform::Win32::FARPROC>(F1)) == FALSE);
				CHECK(CYB::Platform::Win32::IsBadCodePtr(static_cast<CYB::Platform::Win32::FARPROC>(F2)) == FALSE);
#else
				//Ya just gotta BELIIEEVVVEEEEE
#endif
			}
		}
		WHEN("Illegitimate functions are loaded from the library") {
			void* F1(nullptr), * F2(nullptr);
			CHECK_THROWS_AS(F1 = Mod.LoadFunction(FakeLibraryFunctions[0]), CYB::Exception::SystemData);
			CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
			CHECK_THROWS_AS(F2 = Mod.LoadFunction(FakeLibraryFunctions[1]), CYB::Exception::SystemData);
			CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
			THEN("The appropriate exception is thrown") {
				CHECK_COOL_AND_CALM;
				CHECK(F1 == nullptr);
				CHECK(F2 == nullptr);
			}
		}
	}
}

SCENARIO("AutoModules work", "[Platform][Modules][Functional]") {
	GIVEN("A valid system AutoModule definition") {
		CYB::Platform::Modules::AMKernel32* K32(nullptr);
		WHEN("It is instatiated") {
			CHECK_NOTHROW(K32 = new CYB::Platform::Modules::AMKernel32());
			THEN("No exceptions occur") {
				CHECK_COOL_AND_CALM;
			}
			delete K32;
		}
	}
	GIVEN("A valid AutoModule definition not for this system") {
		CYB::Platform::Modules::AMLibC* LibC(nullptr);
		WHEN("It is instatiated") {
			CHECK_NOTHROW(LibC = new CYB::Platform::Modules::AMLibC());
			THEN("No exceptions occur") {
				CHECK_COOL_AND_CALM;
			}
			delete LibC;
		}
	}
}