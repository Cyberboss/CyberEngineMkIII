#pragma once

namespace CYB {
	namespace Platform {
		namespace ModuleDefinitions {
			class AMFake {};
		};
	};
};

#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(KERNEL32, Win32, false, 
	CreateThread,
	WaitForSingleObject, Sleep, SwitchToThread, GetSystemInfo,
	CloseHandle,
	InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, TryEnterCriticalSection, LeaveCriticalSection,
	VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery)
DEFINE_MODULE(Kernel32, Win32, true, DiscardVirtualMemory)
#else
DEFINE_DUMMY_MODULE(KERNEL32)
DEFINE_DUMMY_MODULE(Kernel32)
#endif
