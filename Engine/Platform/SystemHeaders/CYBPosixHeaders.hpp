#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Includes headers for Posix platforms
		namespace Posix {
			//! @cond
#ifndef TARGET_OS_WINDOWS
			#ifdef CYB_BUILDING_TESTS
			typedef ::pid_t pid_t;
			#endif
			#include <errno.h>
			#include <sys/types.h>
			#include <sys/mman.h>
			#include <linux/limits.h>
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
#endif
			//! @endcond
		};
	};
};