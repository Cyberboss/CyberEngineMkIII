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

#include "OSDeclaration.hpp"
#include "Endianess.hpp"
#include "SystemChecks.hpp"
#include "Assert.hpp"

#include "STL.hpp"

#include "BasicTypes.hpp"
#include "Expressions.hpp"

#include "Strings.hpp"

#include "EngineInformation.hpp"

#include "Exception.hpp"

#include "Singleton.hpp"
#include "ParameterPackIndexer.hpp"
#include "Typename.hpp"

#include "Heap.hpp"

#include "Threadable.hpp"