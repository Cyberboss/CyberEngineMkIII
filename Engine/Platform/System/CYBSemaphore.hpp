//! @file Semaphore.hpp Contains the declaration of CYB::API::Semaphore
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief A reliable wait queue
			class Semaphore : private Implementation::Semaphore, public API::Semaphore {
			private:
				unsigned long long FServiceCount,	//!< @brief The number the next waiting thread will take
					FWakeCount,	//!< @brief The number of threads that will be woken
					FSleepCount;	//!< @brief The number of threads currently asleep
			private:
				/*!
					@brief Lock the mutex associated with the CV
					@par Thread Safety
						This function requires no thread safety
				*/
				void Lock(void) noexcept;
				/*!
					@brief Unlock the mutex associated with the CV
					@par Thread Safety
						This function requires the current thread to hold a lock on the mutex
				*/
				void Unlock(void) noexcept;
				
				/*!
					@brief Enter the condition variable
					@par Thread Safety
						This function requires the current thread to hold a lock on the mutex
				*/
				void EnterCV(void) noexcept;

				/*!
					@brief Wake one thread waiting on the condition variable
					@par Thread Safety
						This function requires no thread safety
				*/
				void WakeOne(void) noexcept;
			public:
				/*!
					@brief Construct a Semaphore
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE Thrown if the internal mutex could not be initialized
				*/
				Semaphore();
				//! @copydoc CYB::API::Semaphore::~Semaphore()
				~Semaphore();

				//! @copydoc CYB::API::Semaphore::SignalN()
				void SignalN(const unsigned long long AN) noexcept final override;
				//! @copydoc CYB::API::Semaphore::SignalAll()
				void SignalAll(void) noexcept final override;
				//! @copydoc CYB::API::Semaphore::Wait()
				void Wait(void) noexcept final override;
			};
		};
	};
};