//! @file OSDeclaration.hpp Defines the TARGET_OS_XXX macro for identifying the operating system the program is being compiled for
#pragma once

#ifdef DOXY
//! @brief Should be defined or not by user before all inclusions of CyberEngine.hpp. Enables engine debugging layers. Also ensure definition in unit matches CyberEngine executable you launch with(CyberEngine.exe for no debug, CyberEngineD.exe for debug). Must be defined by user
#define DEBUG
#endif

#define TARGET_OS_WINDOWS //!< @brief Designates a Windows NT based operating system
#define TARGET_OS_MAC	//!< @brief Designates an OSX based operating system
#define TARGET_OS_LINUX //!< @brief Designates a Unix based operating system

#ifndef _WIN32
#undef TARGET_OS_WINDOWS
#endif
#ifndef __APPLE__
#undef TARGET_OS_MAC
#endif
#ifndef __linux__
#undef TARGET_OS_LINUX
#endif