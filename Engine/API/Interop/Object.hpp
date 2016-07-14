//! @file Object.hpp Defines CYB::API::Interop::Object
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			/*!
				@brief Template type for wrapping pointers and treating them as objects. Only works with CyberEngine interface types
				@tparam AInterface The interface to wrap
			*/
			template <class AInterface> class Object {
				static_assert(std::is_base_of<Allocatable, AInterface>::value, "Objects can only be used with engine allocatables");
			private:
				AInterface* FPointer;	//!< @brief The pointer to the managed object
			public:
				/*!
					@brief Construct an object
					@param APointer A pointer to an engine object that can be freed with the current Allocator's Heap
					@par Thread Safety
						This function requires no thread safety
				*/
				Object(AInterface* const APointer) noexcept;
				Object(Object&& AMove) noexcept;	//!< @brief See @ref structors
				Object& operator=(Object&& AMove) noexcept;	//!< @brief See @ref structors
				~Object();	//!< @brief See @ref structors

				/*! 
					@brief Implicit conversion to AInterface
					@par Thread Safety
						This function requires no thread safety
				*/
				AInterface& operator()(void) noexcept;
				/*!
					@brief Implicit conversion to AInterface
					@par Thread Safety
						This function requires no thread safety
				*/
				const AInterface& operator()(void) const noexcept;

				/*!
					@brief Cast an Object to one of it's base classes
					@tparam ANewInterface A base class of AInterface
					@param AUpcastable An xvalue of the current Object
					@return The new upcasted object
					@par Thread Safety
						This function requires no thread safety
				*/
				template <class ANewInterface> static Object<ANewInterface> Upcast(Object&& AUpcastable) noexcept;
			};
		};
	};
};
