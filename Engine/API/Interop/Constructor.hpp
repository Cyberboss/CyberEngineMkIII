//! @file Constructor.hpp Defines CYB::API::Interop::Constructor
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			/*!
				@brief Template for defining the types of parameters for engine object constructors with multiple arguments
				@tparam AType The last in the series of types
				@attention Unseen is a recursive public inheritance of Constructor<ATypes...> due to lackluster Doxygen parsing
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

				/*! 
					@brief Do placement construction of AAllocatable with @p AArgs at @p ALocation
					@tparam AAllocatable The class being allocated
					@tparam AArgs The argument types
					@param ALocation The location to construct AAllocatable
					@param AArguments The arguments of construction before this segment
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
				*/
				template <class AAllocatable, typename... AArgs> void Construct(void* const ALocation, AArgs&&... AArguments);
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@param AParams The remaining pointers
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType&& AParam, ATypes&&... AParams) noexcept;
				/*! 
					@brief Do placement construction of AAllocatable with @p AArgs at @p ALocation
					@tparam AAllocatable The class being allocated
					@param ALocation The location to construct AAllocatable
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
				*/
				template <class AAllocatable> void Construct(void* const ALocation);
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

				/*! 
					@brief Do placement construction of AAllocatable with @p AArgs at @p ALocation
					@tparam AAllocatable The class being allocated
					@tparam AArgs The argument types
					@param ALocation The location to construct AAllocatable
					@param AArguments The arguments of construction before this segment
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
				*/
				template <class AAllocatable, typename... AArgs> void Construct(void* const ALocation, AArgs&&... AArguments);
			public:
				/*!
					@brief Construct an empty Constructor
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor() noexcept;

				/*! 
					@brief Do placement construction of AAllocatable with @p AArgs at @p ALocation
					@tparam AAllocatable The class being allocated
					@param ALocation The location to construct AAllocatable
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
				*/
				template <class AAllocatable> void Construct(void* const ALocation);
				
				/*!
					@brief A best effort validation for the integrity of a Constructor for a given Allocatable
					@return true if the Constructor is valid, false otherwize
					@par Thread Safety
						This function requires no thread safety
				*/
				template <class AAllocatable> bool Valid(void) const noexcept;
			};
			//! @brief Constructor<void> alias
			using EmptyConstructor = Constructor<void>;
			/*!
				@brief Template for defining the types of parameters for engine object constructors
				@tparam AType The last in the series of types
			*/
			template <typename AType> class Constructor<AType> : public EmptyConstructor {
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

				/*! 
					@brief Do placement construction of AAllocatable with @p AArgs at @p ALocation
					@tparam AAllocatable The class being allocated
					@tparam AArgs The argument types
					@param ALocation The location to construct AAllocatable
					@param AArguments The arguments of construction before this segment
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
				*/
				template <class AAllocatable, typename... AArgs> void Construct(void* const ALocation, AArgs&&... AArguments);
			public:
				/*!
					@brief Construct a Constructor
					@param AParam A pointer to a valid AType
					@par Thread Safety
						This function requires no thread safety
				*/
				Constructor(AType&& AParam) noexcept;
				/*! 
					@brief Do placement construction of AAllocatable with @p AArgs at @p ALocation
					@tparam AAllocatable The class being allocated
					@param ALocation The location to construct AAllocatable
					@par Thread Safety
						Dependant on called constructor
					@attention Throws dependant on called constructor
				*/
				template <class AAllocatable> void Construct(void* const ALocation);
			};

			//! @brief Used for object which aren't allocatables
			class NullConstructor : public EmptyConstructor {
			public:
				/*!
					@brief Placement function
					@tparam AArgs The types of the objects arguments
					@param AArguments The object's constructor arguments
					@par Thread Safety
						This function should never be called
				*/
				template <typename... AArgs> NullConstructor(AArgs&&... AArguments) noexcept;
			};
		};
	};
};