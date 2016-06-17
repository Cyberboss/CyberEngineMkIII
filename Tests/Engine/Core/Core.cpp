#include "TestHeader.hpp"

//Heavy whiteboxing below

struct CoreConstructor;

template<> void CYB::Engine::Core::Backdoor<CoreConstructor>(CoreConstructor& AConstructor);

struct CoreConstructor {
	byte FBytes[sizeof(CYB::Engine::Core)];
	const unsigned int FNumArguments;
	const oschar_t* const* const FArguments;
	bool FConstructing;

	CoreConstructor(const unsigned int ANumArguments, const oschar_t* const* const AArguments) :
		FNumArguments(ANumArguments),
		FArguments(AArguments),
		FConstructing(true)
	{
		CYB::Engine::Core::Backdoor(*this);
	}
	~CoreConstructor() {
		FConstructing = false;
		CYB::Engine::Core::Backdoor(*this);
	}
};

template<> void CYB::Engine::Core::Backdoor<CoreConstructor>(CoreConstructor& AConstructor) {
	if (AConstructor.FConstructing)
		new (static_cast<void*>(AConstructor.FBytes)) Core(AConstructor.FNumArguments, AConstructor.FArguments);
	else
		FSingleton->~Core();
}

SCENARIO("Core instantiation", "[Engine][Core][Functional]") {
	Fake::Core::NullifySingleton();
	GIVEN("The core") {
		CoreConstructor* Constructor(nullptr);
		WHEN("It is instantiated") {
			Constructor = new CoreConstructor(0, nullptr);
			THEN("The engine is properly initialized"){
				CHECK(static_cast<void*>(&CYB::Core()) == static_cast<void*>(Constructor->FBytes));
			}
		}
		delete Constructor;
	}
	Fake::Core::ResetToFakeCorePointer();
}

static bool FFakeProcExitRan(false);
unsigned long long FakeProcExit(Fake::SysCalls::Args&) {
	FFakeProcExitRan = true;
	Fake::Core::ResetToFakeCorePointer();	//do this here before we call closehandle in Process' destructor
	return 0;
}


SCENARIO("Basic engine dry run", "[Engine][Core][Behavioural]") {
	GIVEN("A clean execution environment (With a redirected termination call of course)") {
		ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
		Fake::Core::NullifySingleton();
		SysCallOverride Exit(
#ifdef TARGET_OS_WINDOWS
			CYB::Platform::System::Sys::TERMINATE_PROC
#else
			CYB::Platform::System::Sys::EXIT_PROC
#endif
			, FakeProcExit);
		WHEN("The engine is run") {
			CYB::Engine::Core::Run(0, nullptr);
			THEN("Certain things happened") {
				CHECK(FFakeProcExitRan);
			}
		}
		Fake::Core::ResetToFakeCorePointer();
	}
}