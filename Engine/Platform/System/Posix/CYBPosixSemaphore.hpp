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

					/*!
						@brief Wake all threads waiting on the condition variable
						@par Thread Safety
							This function requires the mutex to be locked
					*/
					void WakeAll(void) noexcept;

					/*!
						@brief Wake one thread waiting on the condition variable
						@par Thread Safety
							This function requires the mutex to be locked
					*/
					void WakeOne(void) noexcept;
				};
			};
		};
	};
};