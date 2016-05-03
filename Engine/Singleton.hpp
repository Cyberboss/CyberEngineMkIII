//! @file Singleton.hpp Defines the singleton template class
#pragma once

namespace CYB {
	namespace API {
		/*! 
			@brief A class for creating static singletons
			@tparam AParent The class that will be inheriting from this one to create a singleton
		*/
		template <class AParent> class Singleton {
		protected:
			static AParent* FSingleton;	//!< @brief The singleton object
		protected:
			/*!
				@brief Construct a singleton and initialize it's pointer
				@par Thread Safety
					Access to this function must be synchronized per template instantiation
				@par Exception Safety
					This function does not throw exceptions
			*/
			Singleton() {
				static_assert(std::is_base_of<Singleton<AParent>, AParent>::value, "CYB::API::Singleton may only be used via inheritance");
				Assert(FSingleton == nullptr);
				FSingleton = static_cast<AParent*>(this);
			}
			/*!
				@brief Destroy a singleton and deinitialize it's pointer
			*/
			~Singleton() {
				Assert(FSingleton == static_cast<AParent*>(this));
				FSingleton = nullptr;
			}
		};
	};
};
template <class AParent> AParent* CYB::API::Singleton<AParent>::FSingleton(nullptr);