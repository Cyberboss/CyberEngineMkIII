#include "TestHeader.hpp"

#include <cstring>

static bool FFakeProcExitRan;
static unsigned long long FakeProcExit(Fake::SysCalls::Args&) {
	FFakeProcExitRan = true;
	Fake::Core::ResetToFakeCorePointer();	//do this here before we call closehandle in Process' destructor
	return 1;
}

REDIRECTED_FUNCTION(BadVirtualAlloc, void* const, const unsigned long long, const unsigned long, const unsigned long) {
	return static_cast<void*>(nullptr);
}

REDIRECTED_FUNCTION(BadMMap, void* const, const unsigned long long, int, int, int, const unsigned long long) {
	return reinterpret_cast<void*>(-1);
}

static unsigned long long FakeLoadSym(Fake::SysCalls::Args& AArgs) {
	if (strcmp(static_cast<const char*>(AArgs.FArg2.FPointer), "VirtualAlloc") == 0)
		return reinterpret_cast<unsigned long long>(&BadVirtualAlloc<void>::RedirectedFunction);
	else if (strcmp(static_cast<const char*>(AArgs.FArg2.FPointer), "mmap") == 0)
		return reinterpret_cast<unsigned long long>(&BadMMap<void>::RedirectedFunction);
	else
#ifdef TARGET_OS_WINDOWS
		return reinterpret_cast<unsigned long long>(CYB::Platform::Win32::GetProcAddress(static_cast<CYB::Platform::Win32::HMODULE>(AArgs.FArg1.FPointer), static_cast<const char*>(AArgs.FArg2.FPointer)));
#else
		return reinterpret_cast<unsigned long long>(CYB::Platform::Posix::dlsym(AArgs.FArg1.FPointer, static_cast<const char*>(AArgs.FArg2.FPointer)));
#endif
}

SCENARIO("Core engine dry run", "[Engine][Core][Behavioural]") {
	GIVEN("A clean execution environment (With a redirected termination call of course)") {
		FFakeProcExitRan = false;
		ModuleDependancy<CYB::Platform::Modules::AMKernel32> K32;
		ModuleDependancy<CYB::Platform::Modules::AMLibC> LibC;
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
		WHEN("A fake VM::Reserve pointer is set up") {
			//what we'll do here is hook the syscall for VM::Reserve and return our own
			SysCallOverride GetProc(CYB::Platform::System::Sys::LOAD_SYMBOL, FakeLoadSym);
			AND_THEN("The engine is run") {
				CYB::Engine::Core::Run(0, nullptr);
				THEN("Certain things happened") {
					CHECK(FFakeProcExitRan);
					CHECK_EXCEPTION_CODE(CYB::Exception::Internal::MEMORY_RESERVATION_FAILURE);
				}
			}
		}
		Fake::Core::ResetToFakeCorePointer();
	}
}