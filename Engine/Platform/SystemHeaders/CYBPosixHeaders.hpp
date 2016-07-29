//! @file CYBPosixHeaders.hpp Includes headers for Posix platforms
#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Includes headers for Posix platforms
		namespace Posix {
			//! @cond
#ifndef TARGET_OS_WINDOWS
			#ifdef CYB_BUILDING_TESTS
			using pid_t = ::pid_t;
			#endif
			#include <errno.h>
			#include <sys/types.h>
			#include <sys/mman.h>
			#include <sys/wait.h>
			#include <sys/stat.h>
			using StatStruct = struct stat;
			#include <unistd.h>
#ifdef _WIN32	//for vs debugging
			#include <posix/signal.h>
#else
			#include <signal.h>
#endif
			#include <spawn.h>
			#include <sched.h>
			#include <pthread.h>
			#include <dlfcn.h>
			#include <dirent.h>

			//pwd.h
			struct passwd
			{
				char *pw_name;		/* Username.  */
				char *pw_passwd;		/* Password.  */
				uid_t pw_uid;		/* User ID.  */
				gid_t pw_gid;		/* Group ID.  */
				char *pw_gecos;		/* Real name.  */
				char *pw_dir;			/* Home directory.  */
				char *pw_shell;		/* Shell program.  */
			};
			int getpwuid_r(uid_t uid, struct passwd *pwd,
				char *buf, size_t buflen, struct passwd **result);
			using PasswdStruct = struct passwd;
			using DirStruct = struct dirent;
#endif
			//! @endcond
		};
	};
};

#ifndef TARGET_OS_WINDOWS
//! @brief Support Window's default of UTF-16
using oschar_t = char;
#endif