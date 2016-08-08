//! @file CYBPosixMutex.hpp Defines CYB::Platform::System::Mutex for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				using namespace Posix;
				//! @brief pthread Mutex
				class Mutex {
				protected:
					pthread_mutex_t FMutex;	//!< 
				protected:
					Mutex();
				};
			};
		};
	};
};