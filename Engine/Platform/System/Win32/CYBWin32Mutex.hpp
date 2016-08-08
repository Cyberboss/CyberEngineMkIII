//! @file CYBWin32Mutex.hpp Defines the Win32 implementation for CYB::Platform::System::Mutex
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Contains the CRITICAL_SECTION struct
				class Mutex {
				protected:
					Win32::CRITICAL_SECTION FCriticalSection;	//!< @brief The CRITICAL_SECTION struct
				};
			};
		};
	};
};
