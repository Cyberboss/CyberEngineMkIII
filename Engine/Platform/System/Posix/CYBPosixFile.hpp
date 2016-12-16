//! @file CYBPosixFile.hpp Defines CYB::Platform::System::File for Posix
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			namespace Implementation {
				//! @brief Handles the file descriptor
				class File {
				protected:
					int FDescriptor; //!< @brief The file descriptor
				protected:
					/*!
						@brief Returns the stat struct for the FDescriptor
						@return The stat struct for FDescriptor
						@par Thread Safety
							This function requires no thread safety
					*/
					StatStruct StatFD(void) const;

					//! @brief See @ref structors
					File() noexcept = default;
					//! @brief See @ref structors
					File(File&& AMove) noexcept;
					
					/*!
						@brief Opens a file for access
						@param APath The Path of the File to open
						@param AMode The Mode of opening the file, subsequent operations must respect this
						@param AMethod The Method of handling preexisting files
						@par Thread Safety
							This function requires no thread safety
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p AMode equals Mode::READ and the file could not be opened
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if @p AMode equals Mode::WRITE or Mode::READ_WRITE and the file could not be opened
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p AMethod equals Method::EXIST and the file portion of @p APath does not exist or if it is not valid in other cases.
						@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p AMode, or @p AMethod is invalid
					*/
					void Init(const System::Path& APath, const API::File::Mode AMode, API::File::Method& AMethod);
				};
			};
		};
	};
};