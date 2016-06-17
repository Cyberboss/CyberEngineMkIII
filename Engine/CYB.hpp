//! @file CYB.hpp Precompiled header for inter-engine operations

#define EXPLICIT_CYB_API_USAGE
#define ASSERTION_OVERRIDE
#ifndef ENABLE_TEST_HOOKS
#define ENABLE_TEST_HOOKS
#endif

#include "API/CyberEngine.hpp"

#include "Platform/CYBPlatform.hpp"

#include "Engine/CYBEngine.hpp"