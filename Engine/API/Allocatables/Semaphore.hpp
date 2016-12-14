//! @file Semaphore.hpp Contains the declaration of CYB::API::Semaphore
namespace CYB {
	namespace API {
		/*!
			@brief A reliable wait queue
			@par WARNING
				Using Mutex's incorrectly results in hard to detect undefined behaviour, proceed with caution
		*/
		class Semaphore : public Interop::Allocatable {
		public:
			/*!
				@brief See @ref interstructors
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE Thrown if the internal mutex could not be initialized
			*/
			using Constructor = Interop::EmptyConstructor;
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
			virtual void Wait(void) noexcept = 0;
		};
	};
};
//! @cond
CYB_SET_ALLOCATABLE_ID(Semaphore);
//! @endcond