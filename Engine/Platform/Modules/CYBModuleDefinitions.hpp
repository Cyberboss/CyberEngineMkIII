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
	WaitForSingleObject, Sleep, SwitchToThread, GetSystemInfo, GetLastError,
	CloseHandle,
	InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, TryEnterCriticalSection, LeaveCriticalSection,
	VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery, GetProcessId,
	MultiByteToWideChar, WideCharToMultiByte,
	CreateProcessW, GetExitCodeProcess,
	GetModuleHandleW, GetModuleFileNameW,
	GetTempPathW, GetCurrentDirectoryW,
	FindFirstFileW, FindNextFileW, FindClose,
	CreateDirectoryW, RemoveDirectoryW, GetFileAttributesW, SetFileAttributesW, DeleteFileW)
DEFINE_WINDOWS_MODULE(Kernel32Extended, "kernel32.dll", Win32, true, DiscardVirtualMemory)
DEFINE_WINDOWS_MODULE(Shell, "shell32.dll", Win32, false, ShellExecuteExW, SHGetKnownFolderPath)
DEFINE_WINDOWS_MODULE(Ole32, "Ole32.dll", Win32, false, CoTaskMemFree)
DEFINE_WINDOWS_MODULE(ShellAPI, "shlwapi.dll", Win32, false, PathRemoveFileSpecW, PathCanonicalizeW, PathFileExistsW)

DEFINE_POSIX_MODULE(LibC, LIBC_SO, Posix, false,
	kill, getpid, waitpid, usleep,
	mmap, mprotect, munmap, madvise,
	readlink, mkdir, realpath, rmdir, unlink,
	opendir, readdir_r, closedir,
	getuid, getpwuid_r, getenv, sysconf,
	posix_spawn)
DEFINE_POSIX_MODULE(RT, LIBRT_SO, Posix, false, sched_yield)
DEFINE_POSIX_MODULE(PThread, LIBPTHREAD_SO, Posix, false, 
	pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock,
	pthread_create, pthread_join)