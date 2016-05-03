#include "TestHeader.hpp"

static const char* const ExistingLibrary(
#ifdef TARGET_OS_WINDOWS
	"Kernel32"
#else
	"m"
#endif
);
static const char* const FakeLibrary("Thisreallyshouldntexitsimeancomeon");

SCENARIO("Modules can load a library", "[Platform][Modules]") {
	GIVEN("An existing library") {
		const char* const Library(ExistingLibrary);
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
		const char* const Library(FakeLibrary);
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