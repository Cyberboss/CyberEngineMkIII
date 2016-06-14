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
					@par Exception Safety
						CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE If the Mutex could not be initialized
				*/
				Mutex();
				//! @brief See CYB::API::Mutex::~Mutex
				~Mutex() final override;

				//! @brief See CYB::API::Mutex::Lock
				void Lock(void) const noexcept final override;
				//! @brief See CYB::API::Mutex::TryLock
				bool TryLock(void) const noexcept final override;
				//! @brief See CYB::API::Mutex::Unlock
				void Unlock(void) const noexcept final override;
			};
		};
	};
};
