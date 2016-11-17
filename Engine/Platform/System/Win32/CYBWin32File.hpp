//! @file CYBWin32File.hpp Defines the Win32 implementation for CYB::Platform::System::File
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			class Path;
			namespace Implementation {
				//! @brief The file handle manager
				class File {
				protected:
					Win32::HANDLE FHandle;	//!< @brief The file handle
					API::File::Method FOpenMethod;	//!< @brief The method used to open the file
				protected:
					//! @brief See @ref structors
					File() noexcept = default;
					//! @brief See @ref structors
					File(File&& AMove) noexcept;

					/*!
						@brief Closes the FHandle if it is valid
						@par Thread Safety
							This function requires no thread safety
					*/
					void Close(void) const noexcept;
				};
			};
		};
	};
};