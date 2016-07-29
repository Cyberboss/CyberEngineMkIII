//! @file Constructor.hpp Defines CYB::API::Interop::Constructor
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			/*!
				@brief Template for defining the types of parameters for engine object constructors with multiple arguments
				@tparam AType The last in the series of types
			*/																			//Doxygen too stupid for the recursive parse
			template <typename AType, typename... ATypes> class Constructor : /*! @cond */ public Constructor<ATypes...> /*! @endcond */ {
			public:
				AType FParam;	//!< @brief The pointer to the parameter
			protected:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@param AParams The remaining pointers
					@param ASize The size of the most childish class
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType&& AParam, ATypes&&... AParams, const unsigned short ASize) noexcept;
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@param AParams The remaining pointers
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType&& AParam, ATypes&&... AParams) noexcept;
			};
			//! @brief Used as a base class by all constructors
			template <> class Constructor<void> {
			private:
				const unsigned short FSize;	//!< @brief The number of arguments in the constructor
			protected:
				/*!
					@brief Construct a non-empty Constructor
					@param ASize The size of the most childish class
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(const unsigned short ASize) noexcept;
			public:
				/*!
					@brief Construct an empty Constructor
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor() noexcept;
				
				/*!
					@brief A best effort validation for the integrity of a Constructor for a given Allocatable
					@return true if the Constructor is valid, false otherwize
				*/
				template <class AAllocatable> bool Valid(void) const noexcept;
			};
			/*!
				@brief Template for defining the types of parameters for engine object constructors
				@tparam AType The last in the series of types
			*/
			template <typename AType> class Constructor<AType> : public Constructor<void> {
			public:
				AType FParam;	//!< @brief The pointer to the parameter
			protected:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@param ASize The size of the most childish class
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType&& AParam, const unsigned short ASize) noexcept;
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType&& AParam) noexcept;
			};
			//! @brief Constructor<void> alias
			using EmptyConstructor = Constructor<void>;
		};
	};
};

#include "Constructor.inl"