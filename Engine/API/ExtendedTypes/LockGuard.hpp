//! @file LockGuard.hpp Defines the LockGuard class
#pragma once

namespace CYB {
	namespace API {
		//! @brief A RAII locking mechanism
		class LockGuard {
		private:
			Mutex* FMutex;	//!< @brief A reference to the owned Mutex
		public:
			/*! 
				@brief Construct a LockGuard that will hold a lock on @p AMutex
				@param AMutex The Mutex to lock
				@param APreLocked If the Mutex is already locked by the calling thread this should be true, false otherwise
				@par Thread Safety
					This function will attempt to lock AMutex
			*/
			LockGuard(Mutex& AMutex, const bool APreLocked = false) noexcept;
			LockGuard(const LockGuard&) = delete;	//!< @brief See @ref structors
			LockGuard(LockGuard&& AMove) noexcept;	//!< @brief See @ref structors
			LockGuard& operator=(LockGuard&& AMove) noexcept;	//!< @brief See @ref structors
			//! @brief Destroys the LockGuard and unlocks the Mutex
			~LockGuard();

			/*!
				@brief Releases the lock on Mutex, this same LockGuard can never reaquire it
				@par Thread Safety
					This function will attempt unlock the owned Mutex
			*/
			void Release(void) noexcept;
		};
	}
}

#include "LockGuard.inl"