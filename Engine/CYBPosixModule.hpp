//! @file CYBPosixModule.hpp Defines CYB::Platform::Module for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Module {
			protected:
				void* FModule;
			protected:
				Module(const char* const AModuleName);
				Module(const Module&) = delete;
				Module(Module&& AMove);
				Module& operator=(Module&& AMove);
				~Module();
			};
		};
	};
};

