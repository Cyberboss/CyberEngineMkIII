#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Mutex {
			protected:
				mutable Win32::CRITICAL_SECTION FCriticalSection;
			};
		};
	};
};
