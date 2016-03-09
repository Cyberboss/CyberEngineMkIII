#pragma once
//!	@file Preprocessor.hpp Does the preprocessing required by the Engine
#ifdef DOXY
//! @brief Should be defined or not by user before all inclusions of CyberEngine.hpp. Enables engine debugging layers. Also ensure definition in unit matches CyberEngine executable you launch with(CyberEngine.exe for no debug, CyberEngineD.exe for debug). Must be defined by user
#define DEBUG
//! @brief Define to make localized strings not use plain english in debug mode. Must be defined by user
#define FORCE_LOCALIZATION
#endif

#include "SystemChecks.hpp"
#include "Endianess.hpp"