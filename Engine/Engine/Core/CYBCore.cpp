//! @file CYBCore.cpp Implements engine core

#include "CYB.hpp"

//The engine has several components it needs to start before even thinking about the initial unit

/*
	. Memory Pools: It needs 2 to start
		The main engine heap
		The logger heap
	. Activate the logger
	. Create the core and have it parse the command line. Wrap it with error processing
		. Lock the engine heap and call into the unit to retrieve the function pointers
		. Activate Steam DRM
		. Lock the mutex if necessary
		. Start the ThreadPool
		. Set the engine language
		. Activate the console
		. Replace our icon with the unit's
		. Activate the unit's memory pool

*/

CYB::Engine::Core::Core(const unsigned int ANumArguments, const oschar_t* const* const AArguments):
	FEngineInformation(CreateEngineInformation()),
	FHeap(Parameters::ENGINE_HEAP_INITIAL_COMMIT_SIZE)
{
	static_cast<void>(ANumArguments);
	static_cast<void>(AArguments);
}

CYB::Engine::Core::~Core(void) {

}

CYB::API::EngineInformation CYB::Engine::Core::CreateEngineInformation(void) noexcept {
	return {
		u8"CyberEngine Mark III",
		u8"Copyright Dextraspace Entertainment",
		COPYRIGHT_YEAR,
		VERSION_MAJOR,
		VERSION_MINOR,
		VERSION_REVISION,
		__DATE__,
		__TIME__ " EST",
	};
}

bool CYB::Engine::Core::LaunchUnit(void) noexcept {
	return false;
}

CYB::Engine::Core& CYB::Engine::Core::GetCore(void) noexcept {
	return *FSingleton;
}
void CYB::Engine::Core::Run[[noreturn]](const unsigned int ANumArguments, const oschar_t* const* const AArguments) noexcept {
	try {
		for (Core CyberEngineMarkIII(ANumArguments, AArguments); CyberEngineMarkIII.LaunchUnit(););
	}
	catch (...) {}
	Platform::System::Process::Terminate(Platform::System::Process::GetSelf());
	CYB::API::Assert::HCF();
}
CYB::Engine::Core& CYB::Core(void) noexcept {
	return Engine::Core::GetCore();
}