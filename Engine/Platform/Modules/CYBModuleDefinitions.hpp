//! @file CYBModuleDefinitions.hpp uses the module macros to define AutoModules that will be used by the engine
#pragma once

namespace CYB {
	namespace Platform {
		namespace Modules {
			/*!
				@brief Fake AutoModule used in place of other Platform's AutoModules
				@tparam AUnused Used with __COUNTER__ to create unique types
			*/
			template <unsigned int AUnused> class AMFake {};
		};
	};
};
//! @cond
//Posix generation is weird here

DEFINE_WINDOWS_MODULE(Kernel32, u8"kernel32", Win32, false, true,
	CreateThread,
	WaitForSingleObject, Sleep, SwitchToThread, GetSystemInfo, GetLastError,
	CloseHandle,
	InitializeCriticalSection, DeleteCriticalSection, EnterCriticalSection, TryEnterCriticalSection, LeaveCriticalSection,
	InitializeConditionVariable, WakeConditionVariable, WakeAllConditionVariable, SleepConditionVariableCS,
	VirtualAlloc, VirtualFree, VirtualProtect, VirtualQuery, GetProcessId,
	MultiByteToWideChar, WideCharToMultiByte,
	CreateProcessW, GetExitCodeProcess,
	GetModuleHandleW, GetModuleFileNameW,
	GetTempPathW, GetCurrentDirectoryW,
	FindFirstFileW, FindNextFileW, FindClose,
	CreateDirectoryW, RemoveDirectoryW, GetFileAttributesExW, SetFileAttributesW, DeleteFileW,
	CreateFileW, GetFileSizeEx, SetFilePointerEx, ReadFile, WriteFile,
	AllocConsole, FreeConsole, AttachConsole, WriteConsoleW, SetConsoleMode, GetStdHandle)
DEFINE_WINDOWS_MODULE(Kernel32Extended, u8"kernel32", Win32, true, true, DiscardVirtualMemory)
DEFINE_WINDOWS_MODULE(Shell, u8"shell32", Win32, false, true, ShellExecuteExW, SHGetKnownFolderPath)
DEFINE_WINDOWS_MODULE(Ole32, u8"Ole32", Win32, false, true, CoTaskMemFree)
DEFINE_WINDOWS_MODULE(ShellAPI, u8"shlwapi", Win32, false, true, PathRemoveFileSpecW, PathCanonicalizeW, PathFileExistsW, PathFindFileNameW, PathFindExtensionW, PathIsRootW)

DEFINE_POSIX_MODULE(LibC, LIBC_SO, Posix, false, false,
	kill, getpid, waitpid, usleep,
	mmap, mprotect, munmap, madvise,
	readlink, mkdir, realpath, rmdir, unlink,
	opendir, readdir, closedir,
	getuid, getpwuid_r, getenv, sysconf,
	posix_spawn,
	open, close, lseek, read, write)
DEFINE_POSIX_MODULE(PThread, LIBPTHREAD_SO, Posix, false, false,
	pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock,
	pthread_create, pthread_join,
	pthread_cond_init, pthread_cond_destroy, pthread_cond_wait, pthread_cond_signal, pthread_cond_broadcast)
DEFINE_LINUX_MODULE(RT, LIBRT_SO, Posix, false, false, sched_yield)
DEFINE_OSX_MODULE(System, u8"libSystem", Posix, false, true, sched_yield)
DEFINE_OSX_MODULE(DyLD, u8"/usr/lib/system/libdyld", Posix, false, true, _NSGetExecutablePath)
OVERRIDE_OSX_FUNCTION_NAMES(LibC, 
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	"readdir$INODE64",
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr, nullptr, nullptr)

//! @endcond