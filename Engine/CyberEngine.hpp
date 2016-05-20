//! @file CyberEngine.hpp Precompiled header for engine API
#pragma once

//! @brief Encapsulates the engine in it's entirety
namespace CYB {
	//! @brief Namespace containing the public API define EXPLICIT_API_USAGE to prevent automatic 'using'
	namespace API {};
#ifndef EXPLICIT_CYB_API_USAGE
	using namespace API;
#endif
};

#include "Declarations.hpp"

#include "Types.hpp"

#include "Interop.hpp"

#include "ExtendedTypes.hpp"

#include "Interfaces.hpp"

#include "Allocator.hpp"