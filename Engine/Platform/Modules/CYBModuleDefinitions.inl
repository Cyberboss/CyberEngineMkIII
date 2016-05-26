//! @file CYBModuleDefinitions.inl Defines needed modules used by the platform layer

//! @cond
#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(KERNEL32, Win32, false, 
	CreateThread,
	WaitForSingleObject, Sleep, SwitchToThread, GetSystemInfo,
	CloseHandle,
	InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, TryEnterCriticalSection, LeaveCriticalSection,
	VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery)
DEFINE_MODULE(Kernel32, Win32, true, DiscardVirtualMemory)
#endif
//! @endcond