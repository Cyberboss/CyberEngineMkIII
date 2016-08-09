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
					pthread_mutex_t FMutex;	//!< @brief The mutex
				protected:
					/*!
						@brief Calls pthread_mutex_init on FMutex
						@par Thread Safety
							This function requires no thread safety
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE Thrown if the mutex could not be initialized
					*/
					Mutex();
				};
			};
		};
	};
};