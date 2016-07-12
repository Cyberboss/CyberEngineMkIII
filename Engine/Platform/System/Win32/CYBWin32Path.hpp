//! @file CYBWin32Path.hpp Defines the Win32 implementation for CYB::Platform::System::Path
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Adds a UTF16 accompaniment to the UTF8 string of System::Path
				class Path {
				protected:
					API::String::UTF16 FWidePath;	//!< @brief The UTF16 string
				protected:
					Path() noexcept = default;	//!< @brief See @ref structors
				};
			};
		};
	};
};