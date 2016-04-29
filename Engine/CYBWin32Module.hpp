//! @file CYBWin32Module.hpp Defines CYB::Platform::Module for Win32
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Module {
			protected:
				Win32::HMODULE FModule;
			};
		}
	}
}
