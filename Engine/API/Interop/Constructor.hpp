//! @file Constructor.hpp Defines CYB::API::Interop::Constructor
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			//! @brief Used as a base class by all constructors
			class EmptyConstructor {};
			template <typename AType, typename... ATypes> class Constructor : public Constructor<ATypes...> {
			public:
				AType* const FParam;	//!< @brief The pointer to the parameter
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@param AParams The remaining pointers
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				Constructor(AType* const AParam, ATypes&&... AParams) noexcept :
					Constructor<ATypes...>(std::forward(AParams)...),
					FParam(AParam)
				{}
			};
			/*!
				@brief Template for defining the types of parameters for engine object constructors
				@tparam AType The last in the series of types
			*/
			template <typename AType> class Constructor<AType> : public EmptyConstructor {
			public:
				AType* const FParam;	//!< @brief The pointer to the parameter
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				Constructor(AType* const AParam) noexcept :
					FParam(AParam)
				{}
			};
		};
	};
};