//! @file CYBMutex.hpp Contains the definition for an process unique, fast thread safety mechanism
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief A fast locking, no order guaranteed, mutex
			class Mutex : private Implementation::Mutex, public API::Mutex {
			public:
				/*!
					@brief Constructs a Mutex
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE. Thrown if the Mutex could not be initialized
				*/
				Mutex();
				//! @brief See @ref structors
				Mutex(const Mutex&) = delete;
				//! @copydoc CYB::API::Mutex::~Mutex
				~Mutex() final override;

				//! @copydoc CYB::API::Mutex::Lock
				void Lock(void) noexcept final override;
				//! @copydoc CYB::API::Mutex::TryLock
				bool TryLock(void) noexcept final override;
				//! @copydoc CYB::API::Mutex::Unlock
				void Unlock(void) noexcept final override;
			};
		};
	};
};
