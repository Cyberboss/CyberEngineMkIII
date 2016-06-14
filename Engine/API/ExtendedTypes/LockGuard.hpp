//! @file LockGuard.hpp Defines the LockGuard class
#pragma once

namespace CYB {
	namespace API {
		//! @brief A RAII locking mechanism
		class LockGuard {
		private:
			const Mutex* FMutex;	//!< @brief A reference to the owned mutex
		public:
			/*! 
				@brief Construct a LockGuard that will hold a lock on @p AMutex
				@param AMutex The mutex to lock
				@par Thread Safety
					This function will attempt to lock AMutex
				@par Exception Safety
					This function does not throw exceptions
			*/
			LockGuard(const Mutex& AMutex) noexcept;
			LockGuard(const LockGuard&) = delete;	//!< @brief See @ref structors
			LockGuard(LockGuard&& AMove) noexcept;	//!< @brief See @ref structors
			LockGuard& operator=(LockGuard&& AMove) noexcept;	//!< @brief See @ref structors
			//! @brief Destroys the LockGuard and unlocks the Mutex
			~LockGuard();

			/*!
				@brief Releases the lock on Mutex, this same LockGuard can never reaquire it
				@par Thread Safety
					This function will attempt unlock the owned Mutex
				@par Exception Safety
					This function does not throw exceptions
			*/
			void Release(void) noexcept;
		};
	}
}

#include "LockGuard.inl"