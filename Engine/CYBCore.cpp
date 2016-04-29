//! @file CYBMain.cpp implements engine initialization functions

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
CYB::Engine::Core* CYB::Engine::Core::FCore(nullptr);

CYB::Engine::Core::Core() {

}
CYB::Engine::Core::~Core(void) {

	//Explicitly run destructors

	Platform::Process::Terminate(Platform::Process::GetSelf());
}

bool CYB::Engine::Core::LaunchUnit(void) {
	return false;
}

CYB::Engine::Core& CYB::Engine::Core::GetCore(void) {
	return *FCore;
}
void CYB::Engine::Core::Run[[noreturn]](void) {
	API::Assert(FCore == nullptr);
	{
		Core CyberEngineMarkIII;
		for (; CyberEngineMarkIII.LaunchUnit(););
	}
	CYB::API::HCF();
}
CYB::Engine::Core& CYB::Core(void) {
	return Engine::Core::GetCore();
}