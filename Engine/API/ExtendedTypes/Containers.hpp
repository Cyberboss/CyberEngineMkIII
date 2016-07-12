//! @file Containers.hpp Aliasings of STL containers to use the correct allocator types
#pragma once

namespace CYB {
	namespace API {
		//! @brief Contains usings of STL contianer types
		namespace Container {
			template <typename AType> using Deque = std::deque<AType, Interop::STLAllocator<AType>>;
			template <typename AType> using Vector = std::vector<AType, Interop::STLAllocator<AType>>;
		};
	};
};
