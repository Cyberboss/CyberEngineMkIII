//! @file Containers.hpp Aliasings of STL containers to use the correct allocator types
#pragma once

namespace CYB {
	namespace API {
		namespace Interop { template <class AType> class STLAllocator; };
		//! @brief Contains usings of STL contianer types
		namespace Container {
			//! @brief std::deque algorithms using the CyberEngine's allocator
			template <typename AType> using Deque = std::deque<AType, Interop::STLAllocator<AType>>;
			//! @brief std::vector algorithms using the CyberEngine's allocator
			template <typename AType> using Vector = std::vector<AType, Interop::STLAllocator<AType>>;
			//! @brief std::stack algorithms using the CyberEngine's allocator
			template <typename AType, typename AContainer = Deque<AType>> using Stack = std::stack<AType, AContainer>;
		};
	};
};