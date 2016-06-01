//! @file CYBSystemHeaders.hpp Includes files which include Platform specific headers
#pragma once

#ifdef TARGET_OS_WINDOWS
#include "CYBWin32Headers.hpp"
#else
#include "CYBPosixHeaders.hpp"
#endif

#ifdef TARGET_OS_LINUX
#include "CYBLinuxHeaders.hpp"
#endif

#ifdef TARGET_OS_MAC
#include "CYBOSXHeaders.hpp"
#endif