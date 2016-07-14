//! @file Constructor.hpp Defines CYB::API::Interop::Constructor
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			/*!
				@brief Template for defining the types of parameters for engine object constructors with multiple arguments
				@tparam AType The last in the series of types
			*/
			template <typename AType, typename... ATypes> class Constructor : /*! @cond */ public Constructor<ATypes...> /*! @endcond */ {
			public:
				AType FParam;	//!< @brief The pointer to the parameter
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@param AParams The remaining pointers
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType AParam, ATypes&&... AParams) noexcept :
					Constructor<ATypes...>(std::forward(AParams)...),
					FParam(AParam)
				{}
			};
			//! @brief Used as a base class by all constructors
			template <> class Constructor<void> {};
			/*!
				@brief Template for defining the types of parameters for engine object constructors
				@tparam AType The last in the series of types
			*/
			template <typename AType> class Constructor<AType> : public Constructor<void> {
			public:
				AType FParam;	//!< @brief The pointer to the parameter
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType AParam) noexcept :
					FParam(AParam)
				{}
			};
			typedef Constructor<void> EmptyConstructor;
		};
	};
};