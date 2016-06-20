#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				using namespace Posix;
				class Mutex {
				protected:
					mutable pthread_mutex_t FMutex;
				protected:
					Mutex();
				};
			};
		};
	};
};