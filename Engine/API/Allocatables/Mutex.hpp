//! @file Mutex.hpp Defines the API for CYB::Mutex
#pragma once
namespace CYB {
	namespace API {
		/*!
			@brief A fast locking, no order guaranteed, mutex
			@par WARNING
				Using Mutex's incorrectly results in hard to detect undefined behaviour, proceed with caution
		*/
		class Mutex : private Interop::Allocatable {
		public:
			typedef Interop::EmptyConstructor Constructor;	//!< @brief See @ref interstructors
		public:
			//! @brief Destroy a Mutex. Mutex must be unlocked
			virtual ~Mutex() = default;
			/*!
				@brief Acquire a lock on a Mutex. This will block the current thread until the lock is aquired. The Mutex must not be owned by the current thread
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void Lock(void) const noexcept = 0;
			/*!
				@brief Attempt to acquire a lock on a Mutex. This will not block the current thread. The Mutex must not be owned by the current thread
				@return true if the mutex was acquired, false otherwise
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual bool TryLock(void) const noexcept = 0;
			/*!
				@brief Release a lock on a Mutex. Should only be called after a Lock or TryLock on the same Mutex was aquired in the same thread
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void Unlock(void) const noexcept = 0;
		};
	};
};
CYB_SET_ALLOCATABLE_ID(Mutex)