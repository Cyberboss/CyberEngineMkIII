#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Module {
			protected:
				Win32::HMODULE FModule;
			protected:
				Module(const API::String::CStyle& AModuleName);
				Module(const Module&) = delete;
				Module(Module&& AMove);
				Module& operator=(Module&& AMove);
				~Module();
			};
		}
	}
}
