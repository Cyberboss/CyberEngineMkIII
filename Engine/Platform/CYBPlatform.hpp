//! @file CYBPlatform.hpp Includes platform headers
#pragma once

//! @brief Namespace containing engine Platform functions
namespace Platform {};

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