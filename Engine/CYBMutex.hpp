//! @file CYBMutex.hpp Contains the definition for an process unique, fast thread safety mechanism
#pragma once
namespace CYB {
	namespace Platform {
		//! @brief A fast locking, no order guaranteed, mutex
		class Mutex : private Implementation::Mutex {
		public:
			/*!
				@brief Constructs a Mutex
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE If the Mutex could not be initialized
			*/
			Mutex();
			/*!
				@brief Destroy a Mutex
				@par WARNING
					Potential hard crash due to possiblity of thrown exception: CYB::Exception::SystemData::MUTEX_DESTRUCTION_FAILURE
			*/
			~Mutex();

			/*!
				@brief Acquire a lock on a Mutex. This will block the current thread until the lock is aquired
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			void Lock(void);
			/*!
				@brief Attempt to acquire a lock on a Mutex. This will not block the current thread
				@return true if the mutex was aquired, false otherwise
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			bool TryLock(void);
			/*!
				@brief Release a lock on a Mutex. Should only be called after a Lock on the same Mutex was aquired
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			void Unlock(void);
		};
	};
};
