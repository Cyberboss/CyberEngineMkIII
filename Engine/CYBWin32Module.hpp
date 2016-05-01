//! @file CYBWin32Module.hpp Defines CYB::Platform::Module for Win32
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Module {
			protected:
				Win32::HMODULE FModule;
			protected:
				Module(const char* const AModuleName);
				Module(const Module&) = delete;
				Module(Module&& AMove);
				Module& operator=(Module&& AMove);
				~Module();
			};
		}
	}
}
