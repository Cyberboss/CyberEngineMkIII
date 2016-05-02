//! @file CYBModuleDefinitions.inl Defines needed modules used by the platform layer

namespace CYB {
	namespace Engine {
		template <unsigned int AN, typename... AFunctionTypes> class AutoModule;
	};
};

#ifndef DOXY

#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(Kernel32, Win32, DiscardVirtualMemory)
#endif

#endif