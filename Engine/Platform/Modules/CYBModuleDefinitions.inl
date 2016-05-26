#pragma once

namespace CYB {
	namespace Platform {
		namespace ModuleDefinitions {
			class AMFake {};
		};
	};
};

#ifdef TARGET_OS_WINDOWS
DEFINE_MODULE(Kernel32, "kernel32.dll", Win32, false, 
	CreateThread,
	WaitForSingleObject, Sleep, SwitchToThread, GetSystemInfo,
	CloseHandle,
	InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, TryEnterCriticalSection, LeaveCriticalSection,
	VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery)
DEFINE_MODULE(Kernel32Extended, "kernel32.dll", Win32, true, DiscardVirtualMemory)
#else
DEFINE_DUMMY_MODULE(Kernel32)
DEFINE_DUMMY_MODULE(Kernel32Extended)
#endif

#ifdef TARGET_OS_LINUX
#include <gnu/lib-names.h>
DEFINE_MODULE(C, LIBC_SO, Posix, false, kill, getpid, usleep, mmap, mprotect, munmap, madvise)
DEFINE_MODULE(RT, LIBRT_SO, Posix, false, sched_yield)
DEFINE_MODULE(PThread, LIBPTHREAD_SO, Posix, false, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock, pthread_create, pthread_join)
#elif defined(TARGET_OS_MAC)
#else
DEFINE_DUMMY_MODULE(C)
DEFINE_DUMMY_MODULE(RT)
DEFINE_DUMMY_MODULE(PThread)
#endif