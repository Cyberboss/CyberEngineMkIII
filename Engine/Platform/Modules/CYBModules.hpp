//! @file CYBModules.hpp Encompasses Module functions
#pragma once

#include "CYBModule.hpp"
#include "CYBAutoModule.hpp"
#include "CYBAutoModule.inl"
#include "CYBModuleMacros.hpp"
#include "CYBModuleDefinitions.inl"

#ifdef TARGET_OS_LINUX
#include "../Linux/CYBLinuxModule.inl"
#endif

#ifdef TARGET_OS_MAC
#include "../OSX/CYBOSXModule.inl"
#endif

#include "CYBModuleManager.hpp"