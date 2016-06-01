#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				class Mutex {
				protected:
					mutable Win32::CRITICAL_SECTION FCriticalSection;
				};
			};
		};
	};
};
