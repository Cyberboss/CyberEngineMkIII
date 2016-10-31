//! @file CYBOSXHeaders.hpp Includes headers for OSX platforms
#pragma once
namespace CYB {
	namespace Platform {
		namespace Posix {
#ifdef TARGET_OS_MAC
#define LIBC_SO                        u8"libc.dylib"
#define LIBDL_SO                       u8"libdl.dylib"
#define LIBPTHREAD_SO                  u8"libpthread.dylib"
#define LIBRT_SO                       u8"librt.dylib"
#define thread_local __thread	//TLS hack
			#include <limits.h>
			#include <stdlib.h>
			#include <mach-o/dyld.h>
			#include <sys/dir.h>
#endif
		}
	}
}
#ifdef TARGET_OS_MAC
extern char **environ;
#endif