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
			CYB::Platform::Module* Result(nullptr);
			REQUIRE_NOTHROW(Result = new CYB::Platform::Module(Library));
			THEN("It is loaded successfully") {
				REQUIRE(Result != nullptr);
			}
			delete Result;
		}
	};
	GIVEN("A non existant library") {
		auto& Library(FakeLibrary);
		WHEN("The library name is used to construct a Module") {
			CYB::Platform::Module* Result(nullptr);
			CHECK_THROWS_AS(Result = new CYB::Platform::Module(Library), CYB::Exception::SystemData);
			THEN("It fails to load") {
				CHECK(Result == nullptr);
				CHECK(CYB::Exception::FLastInstantiatedExceptionCode == CYB::Exception::SystemData::MODULE_LOAD_FAILURE);
			}
			delete Result;
		}
	}
};

SCENARIO("Functions can be loaded from modules", "[Platform][Modules][Functional]") {
	GIVEN("A loaded library") {
		CYB::Platform::Module Mod(ExistingLibrary);
		WHEN("Legitimate functions are loaded from the library"){
			void* F1(nullptr),* F2(nullptr);
			CHECK_NOTHROW(F1 = Mod.LoadFunction(ExistingLibraryFunctions[0]));
			CHECK_NOTHROW(F2 = Mod.LoadFunction(ExistingLibraryFunctions[1]));
			THEN("They are not null and valid code locations") {
				CHECK(F1 != nullptr);
				CHECK(F2 != nullptr);
#ifdef TARGET_OS_WINDOWS
				CHECK(CYB::Platform::Implementation::Win32::IsBadCodePtr(static_cast<CYB::Platform::Implementation::Win32::FARPROC>(F1)) == FALSE);
				CHECK(CYB::Platform::Implementation::Win32::IsBadCodePtr(static_cast<CYB::Platform::Implementation::Win32::FARPROC>(F2)) == FALSE);
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
				CHECK(F1 == nullptr);
				CHECK(F2 == nullptr);
			}
		}
	}
}

TEST_CASE("AutoModules work as intended", "[Platform][Modules][Functional]") {
	REQUIRE_NOTHROW(CYB::Platform::ModuleDefinitions::AMKernel32());
	REQUIRE_NOTHROW(CYB::Platform::ModuleDefinitions::AMPThread());
}