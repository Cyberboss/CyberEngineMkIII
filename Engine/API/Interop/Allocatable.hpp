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
					Mutex,	//!< @brief CYB::API::Mutex
					Path,	//!< @brief CYB::API::Path
				};
			public:
				/*!
					@brief Returns the ID of an allocatable object
					@tparam AParent The allocatable object whose ID is being checked
				*/
				template <class AParent> static constexpr ID GetID(void);

				//! @brief See @ref structors
				virtual ~Allocatable() = default;
			};
		};
	};
};