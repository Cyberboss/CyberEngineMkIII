//! @file CYBPosixSemaphore.hpp Defines the Win32 implementation for CYB::Platform::System::File
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief The CS, CV pair handler
				class Semaphore {
				protected:
					pthread_mutex_t FMutex;	//!< @brief The mutex
					pthread_cond_t FCondVar;	//!< @brief The condition variable
				protected:
					/*!
						@brief Sets the initializer values for the pthread structures
						@par Thread Safety
							This function requires no thread safety
					*/
					Semaphore() noexcept;
				};
			};
		};
	};
};