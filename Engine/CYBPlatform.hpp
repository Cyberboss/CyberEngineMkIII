//! @file CYBPlatform.hpp Includes platform headers
#pragma once

//! @brief Namespace containing engine Platform functions
namespace Platform {};

//! @cond
#ifdef TARGET_OS_WINDOWS
#include "CYBWin32Implementation.hpp"
#else
#include "CYBPosixImplementation.hpp"
#endif

#ifdef TARGET_OS_MAC
#include "CYBOSXImplementation.hpp"
#endif

#ifdef TARGET_OS_LINUX
#include "CYBLinuxImplementation.hpp"
#endif
//! @endcond

#include "CYBSyscalls.hpp"
#include "CYBVirtualMemory.hpp"
#include "CYBThread.hpp"
#include "CYBProcess.hpp"
#include "CYBModules.hpp"
#include "CYBMutex.hpp"
//#include "CYBFile.hpp"