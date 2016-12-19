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

thread_local CYB::Engine::Context* CYB::Engine::Core::FCurrentContext;

CYB::Engine::Core::Core(const unsigned int ANumArguments, const oschar_t* const* const AArguments) :
	Singleton<Core>(true),
	FEngineInformation(Parameters::CreateEngineInformation()),
	FHeap(Parameters::ENGINE_HEAP_INITIAL_COMMIT_SIZE),
	FEngineContext(FHeap, true)
{
	static_cast<void>(ANumArguments);
	static_cast<void>(AArguments);
}

CYB::Engine::Core& CYB::Engine::Core::GetCore(void) noexcept {
	return *FSingleton;
}

void CYB::Engine::Core::Run(const unsigned int ANumArguments, const oschar_t* const* const AArguments) noexcept {
	try {
		Core CyberEngineMarkIII(ANumArguments, AArguments);
	}
	catch (...) {}
	Platform::System::Process::GetSelf().Terminate();
}

CYB::Engine::Context& CYB::Engine::Core::CurrentContext(void) noexcept {
	return *FCurrentContext;
}

void CYB::Engine::Core::SetCurrentContext(Context& ANewContext) noexcept {
	FCurrentContext = &ANewContext;
}

CYB::Engine::Core& CYB::Core(void) noexcept {
	return Engine::Core::GetCore();
}