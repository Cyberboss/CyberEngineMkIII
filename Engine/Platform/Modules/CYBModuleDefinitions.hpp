//! @file CYBModuleDefinitions.hpp uses the module macros to define AutoModules that will be used by the engine
#pragma once

namespace CYB {
	namespace Platform {
		namespace Modules {
			//! @brief Fake AutoModule used in place of other Platform's AutoModules
			class AMFake {};
		};
	};
};
//! @cond
//Posix generation is weird here

DEFINE_WINDOWS_MODULE(Kernel32, u8"kernel32.dll", Win32, false,
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
	CreateDirectoryW, RemoveDirectoryW, GetFileAttributesW, SetFileAttributesW, DeleteFileW,
	CreateFileW)
DEFINE_WINDOWS_MODULE(Kernel32Extended, u8"kernel32.dll", Win32, true, DiscardVirtualMemory)
DEFINE_WINDOWS_MODULE(Shell, u8"shell32.dll", Win32, false, ShellExecuteExW, SHGetKnownFolderPath)
DEFINE_WINDOWS_MODULE(Ole32, u8"Ole32.dll", Win32, false, CoTaskMemFree)
DEFINE_WINDOWS_MODULE(ShellAPI, u8"shlwapi.dll", Win32, false, PathRemoveFileSpecW, PathCanonicalizeW, PathFileExistsW, PathFindFileNameW, PathFindExtensionW, PathIsRootW)

DEFINE_POSIX_MODULE(LibC, LIBC_SO, Posix, false,
	kill, getpid, waitpid, usleep,
	mmap, mprotect, munmap, madvise,
	readlink, mkdir, realpath, rmdir, unlink,
	opendir, readdir, closedir,
	open,
	getuid, getpwuid_r, getenv, sysconf,
	posix_spawn)
DEFINE_POSIX_MODULE(PThread, LIBPTHREAD_SO, Posix, false, 
	pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock,
	pthread_create, pthread_join)
DEFINE_LINUX_MODULE(RT, LIBRT_SO, Posix, false, sched_yield)
DEFINE_OSX_MODULE(System, u8"libSystem.dylib", Posix, false, sched_yield)
DEFINE_OSX_MODULE(DyLD, u8"/usr/lib/system/libdyld.dylib", Posix, false, _NSGetExecutablePath)
OVERRIDE_OSX_FUNCTION_NAMES(LibC, 
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	"readdir$INODE64",
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)

//! @endcond