//! @file CYBEntry.cpp The entry point to the program. Sole purpose is to call Main::Entry and return

#include "../Engine/CYB.hpp"

#ifdef TARGET_OS_WINDOWS
#define main wmain	//!< @brief Wide main definition for Windows systems
#endif

/*! 
	@brief Program entry point. Call CYB::Engine::Init::Run
	@param ANumArguments The number of command line arguments
	@param AArguments The command line arguments
	@par Thread Safety
		This function may only be called once by the C runtime
	@par Exception Safety
		This function does not throw exceptions
*/
int main[[noreturn]](const int ANumArguments, const oschar_t* const AArguments) {
	CYB::API::Assert::LessThanOrEqual(0, ANumArguments);
	CYB::Engine::Core::Run(static_cast<unsigned int>(ANumArguments), AArguments);
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