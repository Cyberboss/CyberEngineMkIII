//! @file CYBSystem.hpp Includes platform system headers
#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Contain basic OS operations
		namespace System {
			//! @brief Contains Platform specific OS functions
			namespace Implementation {};
		};
	};
};

//! @cond
#ifdef TARGET_OS_WINDOWS
#include "Win32/CYBWin32SystemImplementation.hpp"
#else
#include "Posix/CYBPosixSystemImplementation.hpp"
#endif

#ifdef TARGET_OS_MAC
#include "OSX/CYBOSXSystemImplementation.hpp"
#endif

#ifdef TARGET_OS_LINUX
#include "Linux/CYBLinuxSystemImplementation.hpp"
#endif
//! @endcond

#include "CYBSyscalls.hpp"
#include "CYBVirtualMemory.hpp"
#include "CYBThread.hpp"
#include "CYBProcess.hpp"
#include "CYBMutex.hpp"
#include "CYBSemaphore.hpp"
#include "CYBPath.hpp"
#include "CYBFile.hpp"
#include "CYBConsole.hpp"