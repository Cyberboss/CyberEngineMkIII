//! @file CYBOSXSyscalls.hpp Defines the Linux kernel Syscall table
#pragma once

namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Sys {
			public:
				enum CallNumber : unsigned long long {
					EXIT = 0x2000000 + 1,
				};
			};
		};
	};
};