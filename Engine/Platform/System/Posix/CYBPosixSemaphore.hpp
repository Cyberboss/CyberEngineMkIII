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
					//! @brief See @ref structors
					Semaphore() noexcept = default;
				};
			};
		};
	};
};