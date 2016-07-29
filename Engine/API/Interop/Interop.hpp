//! @file Interop.hpp Includes files that contain classes required for communitcation between the engine and the unit
#pragma once

namespace CYB {
	namespace API {
		//! @brief Defines interop classes that should not be used individually
		namespace Interop {};
	};
};

#include "Allocatable.hpp"
#include "Constructor.hpp"
#include "Object.hpp"
#include "Allocator.hpp"
#include "STLAllocator.hpp"