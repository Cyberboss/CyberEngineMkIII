#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Mutex {
			protected:
				mutable Posix::pthread_mutex_t FMutex;
			protected:
				Mutex();
			};
		};
	};
};
