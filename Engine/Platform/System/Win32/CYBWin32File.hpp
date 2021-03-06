//! @file CYBWin32File.hpp Defines the Win32 implementation for CYB::Platform::System::File
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief The file handle manager
				class File {
				protected:
					Win32::HANDLE FHandle;	//!< @brief The file handle
				protected:
					//! @brief See @ref structors
					File() noexcept = default;
					//! @brief See @ref structors
					File(File&& AMove) noexcept;
				};
			};
		};
	};
};