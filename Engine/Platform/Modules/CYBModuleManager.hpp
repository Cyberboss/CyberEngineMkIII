//! @file CYBModuleManager.hpp Contains the definition of CYB::Platform::ModuleManager
#pragma once
namespace CYB {
	namespace Platform {
		//! @brief Encapsulates the engine's loaded modules
		class ModuleManager {
		public:
			//Windows
			ModuleDefinitions::AMKERNEL32 FK32;	//!< @brief Kernel32.dll Win7
			ModuleDefinitions::AMKernel32 FK32Extended;	//!< @brief Kernel32.dll Win8.1+, Optional
		};
	};
};