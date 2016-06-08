//! @file CYBEntry.cpp The entry point to the program. Sole purpose is to call Main::Entry and return

#include "../Engine/CYB.hpp"

#ifdef TARGET_OS_WINDOWS
#define main wmain	//!< @brief Wide main definition for Windows systems
#endif

//! @brief Call CYB::Engine::Init::Run and exit
int main(void) {
	CYB::Engine::Core::Run();
}

void CYB::API::Assert::HCF[[noreturn]](void){
#ifdef DEBUG
	BREAK;
#endif
#ifdef TARGET_OS_WINDOWS
	__assume(false);
#else
	__builtin_unreachable();
#endif
}