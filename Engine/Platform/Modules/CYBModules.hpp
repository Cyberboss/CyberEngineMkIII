//! @file CYBModules.hpp Includes Module headers
#pragma once

namespace CYB {
	namespace Platform {
		//! @brief Contains functions for manipulating shared libraries
		namespace Modules {
			//! @brief Contains Platform specific module functions
			namespace Implementation {};
		};
	};
};

#ifdef TARGET_OS_WINDOWS
#include "Win32/CYBWin32ModuleImplementation.hpp"
#else
#include "Posix/CYBPosixModuleImplementation.hpp"
#endif

#include "CYBModule.hpp"
#include "CYBAutoModule.hpp"
#include "CYBAutoModule.inl"
#include "CYBModuleMacros.hpp"
#include "CYBModuleDefinitions.hpp"
#include "CYBModuleManager.hpp"