//! @file CYBModules.hpp Encompasses Module functions
#pragma once

#include "CYBModule.hpp"
#include "CYBAutoModule.hpp"
#include "CYBAutoModule.inl"
#include "CYBModuleMacros.hpp"
#include "CYBModuleDefinitions.inl"

#ifdef TARGET_OS_WINDOWS
#include "../Win32/CYBWin32RequiredModules.hpp"
#else
#include "../Posix/CYBPosixRequiredModules.hpp"
#endif

#ifdef TARGET_OS_LINUX
#include "../Linux/CYBLinuxModule.inl"
#include "../Linux/CYBLinuxRequiredModules.hpp"
#endif

#ifdef TARGET_OS_MAC
#include "../OSX/CYBOSXModule.inl"
#include "../OSX/CYBOSXRequiredModules.hpp"
#endif

#include "CYBModuleManager.hpp"