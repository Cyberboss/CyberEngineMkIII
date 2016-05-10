//! @file CYBWin32Mutex.hpp Defines the implementations for CYB::Platform::Mutex for Posix
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
