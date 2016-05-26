//! @file CYBPlatform.hpp Includes platform headers
#pragma once

namespace CYB {
	//! @brief Namespace containing engine Platform functions
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
#include "Win32/CYBWin32Implementation.hpp"
#else
#include "Posix/CYBPosixImplementation.hpp"
#endif

#ifdef TARGET_OS_MAC
#include "OSX/CYBOSXImplementation.hpp"
#endif

#ifdef TARGET_OS_LINUX
#include "Linux/CYBLinuxImplementation.hpp"
#endif
//! @endcond

#include "CYBSyscalls.hpp"
#include "CYBVirtualMemory.hpp"
#include "CYBThread.hpp"
#include "CYBProcess.hpp"
#include "Modules/CYBModules.hpp"
#include "CYBMutex.hpp"
//#include "CYBFile.hpp"