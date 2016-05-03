//! @file Typename.hpp Defines the Typename template prototype
#pragma once
namespace CYB {
	namespace API {
		/*!
			@brief Gets the typename of a type
			@tparam AType The type to name
		*/
		template <typename AType> class Typename {
		public:
			/*!
				@brief Gets the typename of type AType
				@return The name of the type in a string
			*/
			static constexpr const char* Name(void);
		};
	};
};

//! @brief Macro for auto-naming types
#define ENABLE_TYPENAME(AType, ANamespace) namespace CYB { namespace API { template<> class Typename<ANamespace::AType> { public: constexpr const char* Name(void) { return #AType; }};}}