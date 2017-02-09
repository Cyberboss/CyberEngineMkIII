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
thread_local unsigned long long CYB::Engine::Core::FThreadID(0);

CYB::Engine::Core::Core(const unsigned int ANumArguments, const oschar_t* const* const AArguments) :
	Singleton<Core>(true),
	FEngineInformation(Parameters::CreateEngineInformation()),
	FThreadCounter(0),
	FLogger(FConsole),
	FHeap(Parameters::ENGINE_HEAP_INITIAL_COMMIT_SIZE),
	FEngineContext(FHeap, FLogger, true)
{
	FLogger.Log(API::String::Static(u8"Core created"), Logger::Level::DEV);
	static_cast<void>(ANumArguments);
	static_cast<void>(AArguments);
}

CYB::Engine::Core::~Core() {
	FLogger.Log(API::String::Static(u8"Core shutdown"), Logger::Level::DEV);
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

void CYB::Engine::Core::DefaultContext(void) noexcept {
	FEngineContext.MakeCurrent();
}

CYB::Engine::Core& CYB::Core(void) noexcept {
	return Engine::Core::GetCore();
}

unsigned long long CYB::Engine::Core::ThreadID(void) noexcept {
	if (FThreadID == 0)
		FThreadID = FThreadCounter.fetch_add(1, std::memory_order_relaxed);
	return FThreadID;
}