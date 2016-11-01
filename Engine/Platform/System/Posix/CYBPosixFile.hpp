//! @file CYBPosixFile.hpp Defines CYB::Platform::System::File for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Handles the file descriptor
				class File {
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