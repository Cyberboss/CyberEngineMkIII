//! @file Singleton.hpp Defines the singleton template class
#pragma once

namespace CYB {
	namespace API {
		/*! 
			@brief A class for creating static singletons
			@tparam AParent The class that will be inheriting from this one to create a singleton
		*/
		template <class AParent> class Singleton {
			ENABLE_TEST_HOOKS
		protected:
			static AParent* FSingleton;	//!< @brief The singleton object
		protected:
			/*!
				@brief Optionally initialize the singleton with the current object
				@param AInit If set to true, FSingleton will be set to this
				@par Thread Safety
					Access to this function must be synchronized per template instantiation
			*/
			Singleton(const bool AInit) noexcept;
			//! @brief Destroy a singleton and nullify it's pointer
			~Singleton();
		};
	};
};
#include "Singleton.inl"