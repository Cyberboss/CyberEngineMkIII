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
