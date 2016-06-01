#include "TestHeader.hpp"

using namespace CYB::Engine::Memory;

static unsigned int* FAllocationFunctionCalled;

REDIRECTED_FUNCTION(VirtualAlloc, void* const AAddress, const unsigned long long ASize, const unsigned long AAllocationType, const unsigned long AProtection) {
	++(*FAllocationFunctionCalled);
	return ARedirector::CallOriginal(AAddress, ASize, AAllocationType, AProtection);
}

REDIRECTED_FUNCTION(MMap, void* const AAddress, const unsigned long long ASize, int AProtection, int AFlags, int AFD, const unsigned long long AOffset) {
	++(*FAllocationFunctionCalled);
	return ARedirector::CallOriginal(AAddress, ASize, AProtection, AFlags, AFD, AOffset);
}

SCENARIO("Test basic Heap functions", "[Engine][Memory][Functional]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::Modules::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::Modules::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A basic Heap") {
		auto VA(K32.Redirect<CYB::Platform::Modules::Kernel32::VirtualAlloc, VirtualAlloc>());
		auto MM(LibC.Redirect<CYB::Platform::Modules::LibC::mmap, MMap>());
		REQUIRE(true);
	}
}