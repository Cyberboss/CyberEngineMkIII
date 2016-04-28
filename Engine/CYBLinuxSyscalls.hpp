//! @file CYBLinuxSyscalls.hpp Defines the Linux kernel Syscall table

#pragma once

namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Sys {
			public:
				enum CallNumber : unsigned long long {
					YIELD = 24,
					EXIT = 60,
					KILL = 62,
				};
			};
		};
	};
};