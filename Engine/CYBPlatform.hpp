//! @file CYBPlatform.hpp Includes platform headers
#pragma once

//! @brief Namespace containing engine Platform functions
namespace Platform {};

#ifndef DOXY
#ifdef TARGET_OS_WINDOWS
#include "CYBWin32Implementation.hpp"
#else
#include "CYBPosixImplementation.hpp"
#endif
#endif

#if defined(TARGET_OS_MAC) && !defined(DOXY)
#include "CYBOSXImplementation.hpp"
#endif

#if defined(TARGET_OS_LINUX) && !defined(DOXY)
#include "CYBLinuxImplementation.hpp"
#endif

#include "CYBSyscalls.hpp"
#include "CYBVirtualMemory.hpp"
#include "CYBThread.hpp"
#include "CYBProcess.hpp"