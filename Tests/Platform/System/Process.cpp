#include "TestHeader.hpp"

#ifndef TARGET_OS_WINDOWS
static bool FSysExitCalledProperly(false);
unsigned long long CYB::Platform::System::Sys::DoCall(const CallNumber ACallNumber, const Union64 AArg1) {
	FSysExitCalledProperly = ACallNumber == CallNumber::EXIT && AArg1.FNumber == 0;
	return 0;
}
//don't bother with it
void CYB::Platform::System::Sys::VerifyArgumentCount(const CallNumber ACallNumber, const unsigned long long ANumArgs) {}
#endif

SCENARIO("Getting the running process works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("The running process (me!)") {
		WHEN("Process::GetSelf is called") {
			auto Result(CYB::Platform::System::Process::GetSelf());
			THEN("The process returned is me") {
				CHECK_COOL_AND_CALM;
			}
		}
	}
}

SCENARIO("The Process move constructor works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	GIVEN("A Process") {
		auto Proc(CYB::Platform::System::Process::GetSelf());
		WHEN("The process is moved and move assigned") {
			{
				auto Proc2(std::move(Proc));
				Proc = std::move(Proc2);
			}
			THEN("All is well") {
				CHECK_COOL_AND_CALM;
			}
		}
	}
}

SCENARIO("Process equivalence works", "[Platform][System][Process][Unit]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A Process") {
		auto Proc(CYB::Platform::System::Process::GetSelf());
		WHEN("The process is compared with itself") {
			auto Proc2(CYB::Platform::System::Process::GetSelf());
			THEN("They are the same") {
				CHECK_COOL_AND_CALM;
				CHECK(Proc == Proc2);
			}
		}
	}
}