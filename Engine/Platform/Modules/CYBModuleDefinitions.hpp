#pragma once

namespace CYB {
	namespace Platform {
		namespace Modules {
			class AMFake {};
		};
	};
};

DEFINE_WINDOWS_MODULE(Kernel32, "kernel32.dll", Win32, false,
	CreateThread,
	WaitForSingleObject, Sleep, SwitchToThread, GetSystemInfo,
	CloseHandle,
	InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, TryEnterCriticalSection, LeaveCriticalSection,
	VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery)
DEFINE_WINDOWS_MODULE(Kernel32Extended, "kernel32.dll", Win32, true, DiscardVirtualMemory)

#ifdef TARGET_OS_LINUX
#include <gnu/lib-names.h>
#endif
DEFINE_POSIX_MODULE(LibC, LIBC_SO, Posix, false, kill, getpid, usleep, mmap, mprotect, munmap, madvise)
DEFINE_POSIX_MODULE(RT, LIBRT_SO, Posix, false, sched_yield)
DEFINE_POSIX_MODULE(PThread, LIBPTHREAD_SO, Posix, false, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock, pthread_create, pthread_join)