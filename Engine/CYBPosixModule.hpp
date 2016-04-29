//! @file CYBPosixModule.hpp Defines CYB::Platform::Module for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Module {
			protected:
				void* FModule;
			};
		};
	};
};

