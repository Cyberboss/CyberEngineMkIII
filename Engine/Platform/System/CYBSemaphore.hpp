//! @file Semaphore.hpp Contains the declaration of CYB::API::Semaphore
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief A reliable wait queue
			class Semaphore : public API::Semaphore {
			public:
				/*!
					@brief Construct a Semaphore
					@par Thread Safety
						This function requires no thread safety
				*/
				Semaphore();
				//! @copydoc CYB::API::Semaphore::~Semaphore()
				~Semaphore();
				//! @copydoc CYB::API::Semaphore::SignalN()
				void SignalN(const unsigned long long AN) noexcept final override;
				//! @copydoc CYB::API::Semaphore::SignalAll()
				void SignalAll(void) noexcept final override;
				//! @copydoc CYB::API::Semaphore::Wait()
				void Wait(void) const noexcept final override;
				//! @copydoc CYB::API::Semaphore::WaitN()
				bool WaitN(const unsigned int AMilliseconds) const noexcept final override;
			};
		};
	};
};