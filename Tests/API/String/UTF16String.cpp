#include "TestHeader.hpp"

#include <cstring>

using namespace CYB::API::String;
using namespace CYB::Platform::Win32;

SCENARIO("UTF16 - UTF8 Conversions work", "[Platform][String][UTF16][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
#ifdef TARGET_OS_WINDOWS	//We use the MultiByteToWideChar/WideCharToMultiByte functions on windows, the only system where we need UTF16
	UTF8 Dyn(Static(u8"私は自分のベストを尽くします"));
	const auto Wide(L"私は自分のベストを尽くします");
	GIVEN("A valid UTF8 string") {
		WHEN("It is converted to UTF16") {
			UTF16* U16(nullptr);
			REQUIRE_NOTHROW(U16 = new UTF16(Dyn));
			THEN("The correct result is given") {
				CHECK(wcscmp(Wide, U16->WideData()) == 0);
				CHECK(wcscmp(Wide, U16->WString()) == 0);
			}
			delete U16;
		}
	}
	GIVEN("A valid UTF16 string") {
		WHEN("It is converted to UTF8") {
			UTF8 U8;
			REQUIRE_NOTHROW(U8 = UTF16::ToUTF8(Wide));
			THEN("The correct result is given") {
				CHECK(U8 == Dyn);
			}
		}
	}
#else
	CHECK(true);
#endif
}

#ifdef TARGET_OS_WINDOWS
static unsigned int FCallCount(0);

REDIRECTED_FUNCTION(BadMBTWC, UINT AArg1, DWORD AArg2, LPCCH AArg3, int AArg4, LPWSTR AArg5, int AArg6) {
	++FCallCount;
	if (FCallCount == 2) {
		FCallCount = 0;
		return 0;
	}
	return MultiByteToWideChar(AArg1, AArg2, AArg3, AArg4, AArg5, AArg6);
}

REDIRECTED_FUNCTION(BadWCTMB, UINT AArg1, DWORD AArg2, LPCWCH AArg3, int AArg4, LPSTR AArg5, int AArg6, LPCCH AArg7, LPBOOL AArg8) {
	++FCallCount;
	if (FCallCount == 2) {
		FCallCount = 0;
		return 0;
	}
	return WideCharToMultiByte(AArg1, AArg2, AArg3, AArg4, AArg5, AArg6, AArg7, AArg8);
}
#endif

SCENARIO("UTF16 errors work", "[Platform][String][UTF16][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
#ifdef TARGET_OS_WINDOWS	//We use the MultiByteToWideChar/WideCharToMultiByte functions on windows, the only system where we need UTF16
	GIVEN("Bad conversion calls and valid strings") {
		auto BMBTCWC(K32.Redirect<CYB::Platform::Modules::Kernel32::MultiByteToWideChar, BadMBTWC>());
		auto BWCTMB(K32.Redirect<CYB::Platform::Modules::Kernel32::WideCharToMultiByte, BadWCTMB>());
		UTF8 Dyn(Static(u8"私は自分のベストを尽くします"));
		const auto Wide(L"私は自分のベストを尽くします");
		WHEN("The 8 -> 16 conversion function is called") {
			UTF16* U16(nullptr);
			REQUIRE_THROWS_AS(U16 = new UTF16(Dyn), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
			}
			delete U16;
		}
		WHEN("The 16 -> 8 conversion function is called") {
			UTF8 U8;
			REQUIRE_THROWS_AS(U8 = UTF16::ToUTF8(Wide), CYB::Exception::SystemData);
			THEN("The correct error is thrown") {
				CHECK_EXCEPTION_CODE(CYB::Exception::SystemData::STRING_VALIDATION_FAILURE);
			}
		}
	}
#else
	CHECK(true);
#endif
}