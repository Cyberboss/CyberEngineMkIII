//! @file CYBModuleManager.hpp Contains the definition of CYB::Platform::ModuleManager
#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			//! @brief Encapsulates the engine's loaded modules
			class ModuleManager {
			public:
				//Windows
				AMKernel32 FK32;	//!< @brief Kernel32.dll Win7+
				AMKernel32Extended FK32Extended;	//!< @brief Kernel32.dll Win8.1+, Optional
				AMShell FShell;	//!< @brief shell32.dll Win7+
				AMOle32 FOLE;	//!< @brief Ole32.dll Win2K+
				AMShellAPI FShellAPI;	//!< @brief shlwapi.dll Win7+

				//Posix
				AMLibC FC;	//!< @brief libc
				AMPThread FPThread;	//!< @brief libpthread

				//Linux
				AMRT FRT;	//!< @brief	librt
				
				//OSX
				AMSystem FSystem;	//!< @brief libSystem
				AMDyLD FDyLD;	//!< @brief libdyld
			};
		};
	};
};