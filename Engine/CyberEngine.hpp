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

#include "Preprocessor.hpp"

#include "Types.hpp"

#include "EngineInformation.hpp"

#include "STL.hpp"

#include "Exception.hpp"

#include "Singleton.hpp"
#include "ParameterPackIndexer.hpp"
#include "Typename.hpp"

#include "Heap.hpp"

#include "Threadable.hpp"