//! @file FunctionWrapper.hpp Defines the Function class
#pragma once

namespace CYB {
	namespace API {
		//! @brief std::functions using the CyberEngine's allocator
		template <typename AReturn, typename... AArgs> class Function : public std::function<AReturn(AArgs...)> {
		public:
			//! @brief See http://en.cppreference.com/w/cpp/utility/functional/function/function, ignore options 6-10
			template <typename... AConstructorArgs> Function(AConstructorArgs&&... AArguments);
		};
	};
};

#include "FunctionWrapper.inl"