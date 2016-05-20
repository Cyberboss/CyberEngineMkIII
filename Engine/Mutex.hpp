//! @file Mutex.hpp Defines the API for CYB::Mutex
#pragma once
namespace CYB {
	namespace API {
		//! @brief A fast locking, no order guaranteed, mutex
		class Mutex : private Interop::Allocatable {
		public:
			/*!
				@brief Acquire a lock on a Mutex. This will block the current thread until the lock is aquired
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			virtual void Lock(void) = 0;
			/*!
				@brief Attempt to acquire a lock on a Mutex. This will not block the current thread
				@return true if the mutex was aquired, false otherwise
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			virtual bool TryLock(void) = 0;
			/*!
				@brief Release a lock on a Mutex. Should only be called after a Lock on the same Mutex was aquired
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			virtual void Unlock(void) = 0;
		};
	};
};
CYB_SET_ALLOCATABLE_ID(Mutex)