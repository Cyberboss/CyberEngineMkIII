//! @file Semaphore.hpp Contains the declaration of CYB::API::Semaphore
namespace CYB {
	namespace API {
		//! @brief A reliable wait queue
		class Semaphore : public Interop::Allocatable {
		public:
			using Constructor = Interop::EmptyConstructor;	//!< @brief See @ref interstructors
		public:
			//! @brief Destroy a Semaphore. Semaphore must have no waiters
			virtual ~Semaphore() = default;
			/*!
				@brief Signal N waiters of this semaphore. Will not signal more than are currently waiting
				@param AN the number of waiters to wake
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void SignalN(const unsigned long long AN) noexcept = 0;
			/*!
				@brief Signal all current waiters of this semaphore
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void SignalAll(void) noexcept = 0;
			/*!
				@brief Wait indefinitely for a signal from this semaphore. Will be placed last into the queue
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void Wait(void) const noexcept = 0;
			/*!
				@brief Wait at most @p AMilliseconds for a signal from this semaphore. Will be placed last into the queue
				@param AMilliseconds The maximum number of milliseconds to wait
				@return true if the wait was stopped by a signal, false if the time elapsed
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual bool WaitN(const unsigned int AMilliseconds) const noexcept = 0;
		};
	};
};
//! @cond
CYB_SET_ALLOCATABLE_ID(Semaphore);
//! @endcond