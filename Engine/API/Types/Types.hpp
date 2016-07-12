//! @file Types.hpp Defines global level basic types
#pragma once

typedef unsigned char byte;	//!< @brief It's a byte, 8 bits, etc...

#ifdef UNICODE
typedef wchar_t oschar_t;
#else
typedef char oschar_t;
#endif

#include "Containers.hpp"
#include "String/Strings.hpp"

#include "Typename.hpp"
#include "Singleton.hpp"
#include "ParameterPack.hpp"

#include "EngineInformation.hpp"
#include "Exception.hpp"

#include "Threadable.hpp"
#include "ThreadableTaskset.hpp"