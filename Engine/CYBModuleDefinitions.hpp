//! @file CYBModuleDefinitions.inl Defines needed modules used by the platform layer

#ifndef DOXY

#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(Kernel32, Win32, true, DiscardVirtualMemory)
#else
namespace CYB {
	namespace Platform {
		namespace Implementation {
			namespace Posix {
				void* SomeFakeFunction(void* AA, int AV, pthread_t* c);
			};
		};
	};
};
DEFINE_MODULE(FakeAF, Posix, true, SomeFakeFunction)
#endif





#endif