//! @file CYBPlatform.hpp Includes platform system headers
#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Used to hide platform nitty gritty from the rest of the engine
		namespace Implementation {
			//! @brief Used in places where a platform namespace is required for a cross platform operation
			namespace Fake {
				//! @brief Used in places where a platform implementation function is required for a cross platform operation
				void FakeImpl(void);
			};
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
//#include "CYBFile.hpp"