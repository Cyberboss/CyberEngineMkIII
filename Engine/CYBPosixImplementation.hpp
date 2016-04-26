// !@file CYBWin32Implementation.hpp Includes headers for implementations that use the Windows API
#pragma once

namespace CYB {
	namespace Platform {
		namespace Implementation {
			namespace Posix {
				#include <sys/types.h>
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
