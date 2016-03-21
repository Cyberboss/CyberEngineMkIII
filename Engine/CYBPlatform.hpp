//! @file CYBPlatform.hpp Includes platform headers
#pragma once

#ifdef TARGET_OS_WINDOWS
#include "CYBWin32Implementation.hpp"
#else
#include "CYBPosixImplementation.hpp"
#endif

#if defined(TARGET_OS_MAC) && !defined(DOXY)
#include "CYBOSXImplementation.hpp"
#endif

#if defined(TARGET_OS_LINUX) && !defined(DOXY)
#include "CYBLinuxImplementation.hpp"
#endif

#include "CYBSyscalls.hpp"
#include "CYBThread.hpp"
#include "CYBProcess.hpp"