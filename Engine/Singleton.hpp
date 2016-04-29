//! @file Singleton.hpp Defines the singleton template class
#pragma once

namespace CYB {
	namespace API {
		template <class T> class Singleton {
		protected:
			static T* FSingleton;
		protected:
			Singleton() {
				static_assert(std::is_base_of<Singleton<T>, T>::value, "CYB::API::Singleton may only be used via inheritance");
				Assert(FSingleton == nullptr);
				FSingleton = static_cast<T*>(this);
			}
			~Singleton() {
				Assert(FSingleton == static_cast<T*>(this));
				FSingleton = nullptr;
			}
		};
	};
};
template <class T> T* CYB::API::Singleton<T>::FSingleton(nullptr);