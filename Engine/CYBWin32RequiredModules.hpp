//! @file CYBWin32RequiredModules.hpp Defines modules required from Win32
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class RequiredModules {
			public:
				ModuleDefinitions::AMKernel32 FK32Extended;
			};
		};
	};
};