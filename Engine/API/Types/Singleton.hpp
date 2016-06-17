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
				@brief Construct a singleton and initialize it's pointer
				@par Thread Safety
					Access to this function must be synchronized per template instantiation
			*/
			Singleton() noexcept;
			//! @brief Destroy a singleton and nullify it's pointer
			~Singleton();
		};
	};
};
#include "Singleton.inl"