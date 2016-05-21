#pragma once
namespace CYB {
	namespace Platform {
		namespace Implementation {
			class Mutex {
			protected:
				Posix::pthread_mutex_t FMutex;
			protected:
				Mutex();
			};
		};
	};
};
