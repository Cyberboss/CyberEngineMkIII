//! @file Typename.hpp Defines the Typename template prototype
#pragma once
namespace CYB {
	namespace API {
		template <typename T> class Typename {
		public:
			static constexpr const char* Name(void);
		};
	};
};

#define ENABLE_TYPENAME(AType) namespace CYB { namespace API { template<> class Typename<AType> { public: constexpr const char* Name(void) { return #AType; }};}}