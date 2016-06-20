//! @file OSDeclaration.hpp Defines the TARGET_OS_XXX macro for identifying the operating system the program is being compiled for
#pragma once

#ifdef DOXY
//! @brief Should be defined or not by user before all inclusions of CyberEngine.hpp. Enables engine debugging layers. Also ensure definition in unit matches CyberEngine executable you launch with(CyberEngine.exe for no debug, CyberEngineD.exe for debug). Must be defined by user
#define DEBUG
#undef DEBUG
#endif

//#define FAKE_POSIX	
#define TARGET_OS_WINDOWS //!< @brief Designates a Windows NT based operating system
#define TARGET_OS_MAC	//!< @brief Designates an OSX based operating system
#define TARGET_OS_LINUX //!< @brief Designates a Unix based operating system

#if !defined(_WIN32) || defined(FAKE_POSIX)
#undef TARGET_OS_WINDOWS
#endif
#ifndef __APPLE__
#undef TARGET_OS_MAC
#endif
#if !defined(__linux__) && !defined(FAKE_POSIX)
#undef TARGET_OS_LINUX
#endif

//! @cond
//This is an alternative to noexcept(!CYB::API::Platform::IsDebug()) because at some points this can fuck up the compiler
#ifdef _WIN32	//TODO remove this once the VS team fixes the compiler https://connect.microsoft.com/VisualStudio/feedback/details/2838537/ice-when-calling-pure-virtual-function-with-constexpr-noexcept-specifier-with-at-least-1-parameter
#ifndef DEBUG
#define RELEASE_NOEXCEPT noexcept
#else
#define RELEASE_NOEXCEPT
#endif
#else
#define RELEASE_NOEXCEPT noexcept(!CYB::API::Platform::IsDebug())
#endif
//! @endcond

namespace CYB {
	namespace API {
		//! @brief Strong typing for Platform::Identifier
		class Platform {
		public:
			//! @brief A bitmask platform identifier. Should be preferred over the macros whenever possible
			enum Identifier : unsigned int {
				ALL = 0xFFFFFFFF,	//!< @brief All platforms
				POSIX = 1,	//!< @brief Posix platforms
				LINUX = 3,	//!< @brief Linux platforms
				OSX = 5,	//!< @brief OSX platforms
				WINDOWS = 2,	//!< @brief Win32 platforms
			};
		public:
			/*!
				@brief Get the current Platform being compiled on
				@return The Identifier for the current Platform
			*/
			static constexpr Identifier Current(void);
			static constexpr bool IsDebug(void);
			static constexpr bool IsTest(void);
		};
	}
}

#include "OSDeclaration.inl"