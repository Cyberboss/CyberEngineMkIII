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
				CHECK(Result != nullptr);
			}
			delete Result;
		}
	};
	GIVEN("A non existant library") {
		auto& Library(FakeLibrary);
		WHEN("The library name is used to construct a Module") {
			CYB::Platform::Modules::Module* Result(nullptr);
			CHECK_THROWS_AS(Result = new CYB::Platform::Modules::Module(Library), CYB::Exception::Internal);
			THEN("It fails to load") {
				CHECK(Result == nullptr);
				CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MODULE_LOAD_FAILURE);
			}
			delete Result;
		}
	}
};

SCENARIO("Modules can load functions", "[Platform][Modules][Functional]") {
	GIVEN("A loaded library") {
		CYB::Platform::Modules::Module Mod(ExistingLibrary);
		WHEN("Legitimate functions are loaded from the library"){
			void* F1(nullptr),* F2(nullptr);
			CHECK_NOTHROW(F1 = Mod.LoadFunction(ExistingLibraryFunctions[0]));
			CHECK_NOTHROW(F2 = Mod.LoadFunction(ExistingLibraryFunctions[1]));
			THEN("They are not null and valid code locations") {
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
			CHECK_THROWS_AS(F1 = Mod.LoadFunction(FakeLibraryFunctions[0]), CYB::Exception::Internal);
			CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE);
			CHECK_THROWS_AS(F2 = Mod.LoadFunction(FakeLibraryFunctions[1]), CYB::Exception::Internal);
			CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE);
			THEN("The appropriate exception is thrown") {
				CHECK(F1 == nullptr);
				CHECK(F2 == nullptr);
			}
		}
	}
}

SCENARIO("Modules of the automatic variety work", "[Platform][Modules][Functional]") {
	GIVEN("A valid/invalid system AutoModule definition") {
		CYB::Platform::Modules::AMKernel32* K32(nullptr);
		WHEN("It is instatiated") {
			REQUIRE_NOTHROW(K32 = new CYB::Platform::Modules::AMKernel32());
			THEN("No exceptions occur") {
				CHECK(true);
			}
			delete K32;
		}
	}
	GIVEN("A valid/invalid AutoModule definition not for this system") {
		CYB::Platform::Modules::AMLibC* LibC(nullptr);
		WHEN("It is instatiated") {
			REQUIRE_NOTHROW(LibC = new CYB::Platform::Modules::AMLibC());
			THEN("No exceptions occur") {
				CHECK(true);
			}
			delete LibC;
		}
	}
	GIVEN("A valid/invalid AutoModule") {
		CYB::Platform::Modules::AMKernel32 K32;
		CYB::Platform::Modules::AMRT RT;
		CYB::Platform::Modules::AMSystem System;
		WHEN("It is moved"){
			{
				auto K322(std::move(K32));
				K32 = std::move(K322);
			}
			{
				auto RT2(std::move(RT));
				RT = std::move(RT2);
			}
			{
				auto Sys2(std::move(System));
				System = std::move(Sys2);
			}
			THEN("All is well") {
				CHECK(true);
			}
		}
		WHEN("Call are made into them") {
#ifdef TARGET_OS_WINDOWS
			K32.Call<CYB::Platform::Modules::Kernel32::SwitchToThread>();
#else
#ifdef TARGET_OS_LINUX
			RT.Call<CYB::Platform::Modules::RT::sched_yield>();
#else
			System.Call<CYB::Platform::Modules::System::sched_yield>();
#endif
#endif
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}

SCENARIO("Module move constructor and move assignment operator works", "[Platform][Modules][Unit]") {
	GIVEN("A valid module"){
		CYB::Platform::Modules::Module Mod(ExistingLibrary);
		WHEN("The module is moved") {
			{
				auto Mod2(std::move(Mod));
				Mod = std::move(Mod2);
			}
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
}
namespace CYB {
	namespace Platform {
		namespace Win32 {
			void FakeFunctionThatDoesNotExist(void);
		};
		namespace Posix {
			void FakeFunctionThatDoesNotExist(void);
		};
	};
};
DEFINE_WINDOWS_MODULE(FakeKernel32, "kernel32.dll", Win32, true, SwitchToThread, FakeFunctionThatDoesNotExist)
DEFINE_POSIX_MODULE(FakeC, LIBC_SO, Posix, true, mmap, FakeFunctionThatDoesNotExist)

SCENARIO("Module optional functions work", "[Platform][Modules][Unit]") {
	GIVEN("A valid optional module with some fake functions") {
		CYB::Platform::Modules::AMFakeKernel32* TestMod1(nullptr);
		CYB::Platform::Modules::AMFakeC* TestMod2(nullptr);
		WHEN("The module is initialized") {
			REQUIRE_NOTHROW(TestMod1 = new CYB::Platform::Modules::AMFakeKernel32());
			REQUIRE_NOTHROW(TestMod2 = new CYB::Platform::Modules::AMFakeC());
			THEN("Some functions loaded successfully") {
#ifdef TARGET_OS_WINDOWS
				CHECK(TestMod1->Loaded(CYB::Platform::Modules::FakeKernel32::SwitchToThread));
				CHECK_FALSE(TestMod1->Loaded(CYB::Platform::Modules::FakeKernel32::FakeFunctionThatDoesNotExist));
#else
				CHECK(TestMod2->Loaded(CYB::Platform::Modules::FakeC::mmap));
				CHECK_FALSE(TestMod2->Loaded(CYB::Platform::Modules::FakeC::FakeFunctionThatDoesNotExist));
#endif
			}
			delete TestMod1;
			delete TestMod2;
		}
	}
}