// !@file CYBWin32Implementation.hpp Includes headers for implementations that use the Windows API
#pragma once

namespace CYB {
	namespace Platform {
		namespace Implementation {
			namespace Posix {
				#ifdef CYB_BUILDING_TESTS
				typedef ::pid_t pid_t;
				#endif
				#include <errno.h>
				#include <sys/types.h>
				#include <sys/mman.h>
				#include <unistd.h>
				#include <signal.h>
				#include <sched.h>
				#include <pthread.h>
			};
		};
	};
};

#include "CYBPosixThread.hpp"
#include "CYBPosixProcess.hpp"
#include "CYBPosixVirtualMemory.hpp"
#include "CYBPosixModule.hpp"
