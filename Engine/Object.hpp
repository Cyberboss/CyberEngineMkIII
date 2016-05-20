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
				static_assert(std::is_base_of<Allocatable, AInterface>::value_type, "Objects can only be used with engine allocatables");
			private:
				AInterface* FPointer;	//!< @brief The pointer to the managed object
			public:
				/*!
					@brief Construct an object
					@param APointer A pointer to an engine object that can be freed with the current Allocator's Heap
				*/
				Object(AInterface* const APointer);
				Object(Object&& AMove);	//!< @brief See @ref structors
				Object& operator=(Object&& AMove);	//!< @brief See @ref structors
				~Object();	//!< @brief See @ref structors

				//! @brief Implicit conversion to AInterface
				AInterface& operator()(void);
				//! @brief Implicit conversion to const AInterface
				const AInterface& operator()(void) const;
			};
		};
		using Interop::Object;
	};
};
