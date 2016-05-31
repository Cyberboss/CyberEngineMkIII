#include "TestHeader.hpp"

static unsigned int* FAllocationFunctionCalled;
template <class ARedirector> class VirtualAlloc {
public:
	static auto Redirect(void* const AAddress, const unsigned long long ASize, const unsigned long AAllocationType, const unsigned long AProtection) {
		++(*FAllocationFunctionCalled);
		return ARedirector::CallOriginal(AAddress, ASize, AAllocationType, AProtection);
	}
};

template <class ARedirector> class MMap{
public:
	static auto Redirect(void* const AAddress, const unsigned long long ASize, int AProtection, int AFlags, int AFD, const unsigned long long AOffset) {
		++(*FAllocationFunctionCalled);
		return ARedirector::CallOriginal(AAddress, ASize, AProtection, AFlags, AFD, AOffset);
	}
};

SCENARIO("Test basic Heap functions", "[Memory][Functional]") {
	ModuleDependancy<CYB::API::Platform::WINDOWS, CYB::Platform::ModuleDefinitions::AMKernel32> K32(CYB::Core().FModuleManager.FK32);
	ModuleDependancy<CYB::API::Platform::POSIX, CYB::Platform::ModuleDefinitions::AMLibC> LibC(CYB::Core().FModuleManager.FC);
	GIVEN("A basic Heap") {
		auto VA(K32.Redirect<CYB::Platform::ModuleDefinitions::Kernel32::VirtualAlloc, VirtualAlloc>());
		auto MM(LibC.Redirect<CYB::Platform::ModuleDefinitions::LibC::mmap, MMap>());
		REQUIRE(true);
	}
}