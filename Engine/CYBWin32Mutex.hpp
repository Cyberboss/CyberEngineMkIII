//! @file CYBWin32Mutex.hpp Defines the implementations for CYB::Platform::Mutex for Win32
#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Mutex {
			protected:
				Win32::CRITICAL_SECTION FCriticalSection;
			};
		};
	};
};
