#include "TestHeader.hpp"

static const CYB::API::String::Static ExistingLibrary(
#ifdef TARGET_OS_WINDOWS
	u8"Kernel32"
#else
	u8"dl"
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

SCENARIO("Modules can load a library", "[Platform][Modules]") {
	GIVEN("An existing library") {
		auto& Library(ExistingLibrary);
		WHEN("The library name is used to construct a Module") {
			CYB::Platform::Module* Result(nullptr);
			bool Error(false);
			try {
				Result = new CYB::Platform::Module(Library);
			}
			catch (...) {
				Error = true;
			}
			THEN("It is loaded successfully") {
				REQUIRE(!Error);
				REQUIRE(Result != nullptr);
			}
			delete Result;
		}
	};
	GIVEN("A non existant library") {
		auto& Library(FakeLibrary);
		WHEN("The library name is used to construct a Module") {
			CYB::Platform::Module* Result(nullptr);
			bool Error(false);
			try {
				Result = new CYB::Platform::Module(Library);
			}
			catch (...) {
				Error = true;
			}
			THEN("It fails to load") {
				REQUIRE(Error);
				REQUIRE(Result == nullptr);
			}
			delete Result;
		}
	}
};

SCENARIO("Functions can be loaded from modules", "[Platform][Modules]") {
	GIVEN("A loaded library") {
		CYB::Platform::Module Mod(ExistingLibrary);
		WHEN("Legitimate functions are loaded from the library"){
			auto F1(Mod.LoadFunction(ExistingLibraryFunctions[0])), F2(Mod.LoadFunction(ExistingLibraryFunctions[1]));
			THEN("They are not null and valid code locations") {
				REQUIRE(F1 != nullptr);
				REQUIRE(F2 != nullptr);
#ifdef TARGET_OS_WINDOWS
				REQUIRE(CYB::Platform::Implementation::Win32::IsBadCodePtr(static_cast<CYB::Platform::Implementation::Win32::FARPROC>(F1)) == FALSE);
				REQUIRE(CYB::Platform::Implementation::Win32::IsBadCodePtr(static_cast<CYB::Platform::Implementation::Win32::FARPROC>(F2)) == FALSE);
#else
				//Ya just gotta BELIIEEVVVEEEEE
#endif
			}
		}
		WHEN("Illegitimate functions are loaded from the library") {
			bool Error1(false), Error2(false);
			void* F1(nullptr), *F2(nullptr);
			try {
				F1 = Mod.LoadFunction(FakeLibraryFunctions[0]);
			}
			catch (CYB::Exception::SystemData AException) {
				Error1 = AException.FErrorCode == CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE;
			}
			try {
				F2 = Mod.LoadFunction(FakeLibraryFunctions[1]);
			}
			catch (CYB::Exception::SystemData AException) {
				Error2 = AException.FErrorCode == CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE;
			}

			THEN("The appropriate exception is thrown") {
				REQUIRE(Error1);
				REQUIRE(Error2);
				REQUIRE(F1 == nullptr);
				REQUIRE(F2 == nullptr);
			}
		}
	}
}

#ifndef TARGET_OS_WINDOWS
DEFINE_MODULE(dl, Posix, false, dlopen, dlclose, dlsym)
#endif

TEST_CASE("AutoModules work as intended", "[Platform][Modules]") {
#ifdef TARGET_OS_WINDOWS
	CYB::Platform::ModuleDefinitions::AMKernel32();
#else
	CYB::Platform::ModuleDefinitions::AMdl();
#endif
}