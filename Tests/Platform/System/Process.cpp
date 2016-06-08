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
	GIVEN("The running process (me!)") {
		WHEN("Process::GetSelf is called") {
			auto Result(CYB::Platform::System::Process::GetSelf());
			THEN("The process returned is me") {
				CHECK_COOL_AND_CALM;
			}
		}
	}
}