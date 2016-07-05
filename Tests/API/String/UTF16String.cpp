#include "TestHeader.hpp"

#include "../Engine/Platform/CYBUTF16String.inl"

#include <cstring>

using namespace CYB::API::String;

SCENARIO("UTF16 - UTF8 Conversions work", "[Platform][String][UTF16][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
#ifdef TARGET_OS_WINDOWS	//We use the MultiByteToWideChar/WideCharToMultiByte functions on windows, the only system where we need UTF16
	GIVEN("A valid UTF8 string") {
		UTF8 Dyn(Static(u8"私は自分のベストを尽くします"));
		WHEN("It is converted to UTF16") {
			UTF16* U16(nullptr);
			REQUIRE_NOTHROW(U16 = new UTF16(Dyn));
			THEN("All is well") {
				CHECK(true);
			}
			delete U16;
		}
	}
	GIVEN("A valid UTF16 string") {
		const auto Wide(L"私は自分のベストを尽くします");
		WHEN("It is converted to UTF8") {
			UTF8 U8;
			REQUIRE_NOTHROW(U8 = UTF16::ToUTF8(Wide));
			THEN("All is well") {
				CHECK(true);
			}
		}
	}
#else
	CHECK(true);
#endif
}