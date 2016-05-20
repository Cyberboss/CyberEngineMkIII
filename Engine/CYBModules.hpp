//! @file CYBModules.hpp Encompasses Module functions
#pragma once

#include "CYBModule.hpp"
#include "CYBAutoModule.hpp"
#include "CYBAutoModule.inl"
#include "CYBModuleMacros.hpp"
#include "CYBModuleDefinitions.inl"

#ifdef TARGET_OS_WINDOWS
#include "CYBWin32RequiredModules.hpp"
#else
#include "CYBPosixRequiredModules.hpp"
#endif

#ifdef TARGET_OS_LINUX
#include "CYBLinuxModule.hpp"
#include "CYBLinuxRequiredModules.hpp"
#endif

#ifdef TARGET_OS_MAC
#include "CYBOSXModule.hpp"
#include "CYBOSXRequiredModules.hpp"
#endif

#include "CYBModuleManager.hpp"