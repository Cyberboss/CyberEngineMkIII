//! @file Allocatable.hpp Defines CYB::API::Allocatable
#pragma once
namespace CYB {
	namespace API {
		namespace Interop {
			//! @brief Defines the base class for allocatable engine objects
			class Allocatable {
			public:
				//! @brief Defines the IDs of allocatable engine objects
				enum class ID {
					NULL_ID,	//!< @brief Used in case of error
					File,	//!< @brief CYB::API::File
					Mutex,	//!< @brief CYB::API::Mutex
					Path,	//!< @brief CYB::API::Path
					Semaphore,	//!< @brief CYB::API::Semaphore
				};

				//! @brief Override in descendants
				using Constructor = NullConstructor;
				//! @brief Override in descendants
				using CopyConstructor = NullConstructor;
			public:
				/*!
					@brief Returns the ID of an allocatable object
					@tparam AParent The allocatable object whose ID is being checked
					@return The ID of AParent
				*/
				template <class AParent> static constexpr ID GetID(void);

				//! @brief See @ref structors
				virtual ~Allocatable() = default;
			};
		};
	};
};

//! @def Used for tagging classes that are allocatables
#define CYB_SET_ALLOCATABLE_ID(AClass)\
template<> constexpr CYB::API::Interop::Allocatable::ID CYB::API::Interop::Allocatable::GetID<CYB::API::AClass>(void) {\
	return ID::AClass;\
}