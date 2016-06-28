//! @file CYBModuleManager.hpp Contains the definition of CYB::Platform::ModuleManager
#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			//! @brief Encapsulates the engine's loaded modules
			class ModuleManager {
			public:
				//Windows
				AMKernel32 FK32;	//!< @brief Kernel32.dll Win7
				AMKernel32Extended FK32Extended;	//!< @brief Kernel32.dll Win8.1+, Optional
				AMShellAPI FShellAPI;

				//Posix
				AMLibC FC;	//!< @brief libc
				AMRT FRT;	//!< @brief	librt
				AMPThread FPThread;	//!< @brief libpthread
			};
		};
	};
};