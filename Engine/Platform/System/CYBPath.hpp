//! @file CYBPath.hpp Defines the Path interface
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			/*!
				@brief Used for manipulating Paths. Paths will always exist either as a file or directory. Paths are '/' delimited when forming though may not be while retrieving. File names ".." will ascend a directory and '.' represents a no-op
				@attention Only UTF-8 encodedable paths are supported, paths lengths may not exceed 256 BYTES, and directory names may not exceed 248 characters. Symlinks are always resolved on posix systems, but never on Windows systems. This is a user problem and should cause no errors so long as they do not reoganize the installation files. Note that the recursive folder creation and deletion options attempt to fully adhere to the strong guarantee. But, due to the nature of filesystem race conditions, this is impossible.
			*/
			class Path : public Implementation::Path, public API::Path {	//impl has to be public due to File using the wondows wide string cache
			private:
				API::String::UTF8 FPath;	//!< @brief The underlying string
			private:
				/*!
					@brief Get the string path of a SystemPath
					@param ADirectory The type of SystemPath to look up
					@return The string path of @p ADirectory
					@par Thread Safety
						Use of SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static API::String::UTF8 LocateDirectory(const SystemPath ADirectory);
				/*!
					@brief Get the string path of SystemDirectory::RESOURCE
					@return The string path of SystemDirectory::RESOURCE
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. If an invalid value for @p ADirectory is given
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static API::String::UTF8 GetResourceDirectory(void);
				
				/*!
					@brief Ensure the existance of a single directory. Path's before it must exist
					@param APath The path to the directory that will be created. Last token must not be ".."
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the directory could not be created
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static void CreateDirectory(const API::String::UTF8& APath);
				/*!
					@brief Try and create a set of directories
					@param AExistingPath The path to create the first directory in
					@param APaths Names of directories to recusively create. None of these should exist. None of them should be ".."
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the directories could not be created
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static void CreateDirectories(const API::String::UTF8& AExistingPath, const API::Container::Deque<API::String::UTF8>& APaths);
				
				/*!
					@brief Ensure the non-existance of a single file. Path's before it must exist. Directory must be empty
					@param APath The path to the file that will be deleted
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the directory could not be deleted
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static void DeleteFile(const API::String::UTF8& APath);
				/*!
					@brief Ensure the non-existance of a single directory. Path's before it must exist. Directory must be empty
					@param APath The path to the directory that will be deleted. Last token must not be ".."
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the directory could not be deleted
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::DIRECTORY_NOT_EMPTY. Thrown if the directory is not empty
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static void DeleteDirectory(const API::String::UTF8& APath);

				/*!
					@brief Evaluates '..' and '.' references within the path
					@param[out] APath The path to evaluate
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::PATH_EVALUATION_FAILURE. If the path could not be evaluated
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				static void Evaluate(API::String::UTF8& APath);

				/*!
					@brief Verifys the path pointed to exists
					@param APath The path to check
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE If some part of the new path is not readable with the current permissions
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				bool Verify(const API::String::UTF8& APath) const;

				/*!
					@brief Sets the current Path string
					@param APath The string to set the path to
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::FAILED_TO_CONVERT_UTF16_STRING. Thrown if windows failed to convert the string
				*/
				void SetPath(API::String::UTF8&& APath);
			public:
				/*!
					@brief Get the Path of a given SystemPath
					@param ADirectory The type of SystemPath to look up
					@par Thread Safety
						Use of CYB::Platform::Path::SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. If an invalid value for @p ADirectory is given
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
				*/
				Path(const SystemPath ADirectory);
				Path(const Path& ACopy) = default;
				Path(Path&& AMove) noexcept = default;
				Path& operator=(Path&& AMove) noexcept = default;

				//! @copydoc CYB::API::Path::Append()
				bool Append(const API::String::UTF8& AAppendage, const bool ACreateIfNonExistant, const bool ACreateRecursive) final override;
				//! @copydoc CYB::API::Path::NavigateToParentDirectory()
				void NavigateToParentDirectory(void) final override;
				
				//! @copydoc CYB::API::Path::Delete()
				void Delete(const bool ARecursive) const final override;

				//! @copydoc CYB::API::Path::IsDirectory()
				bool IsDirectory(void) const final override;
				//! @copydoc CYB::API::Path::IsFile()
				bool IsFile(void) const final override;

				//! @copydoc CYB::API::Path::CanRead()
				bool CanRead(void) const final override;
				//! @copydoc CYB::API::Path::CanWrite()
				bool CanWrite(void) const final override;
				//! @copydoc CYB::API::Path::CanExecute()
				bool CanExecute(void) const final override;

				//! @copydoc CYB::API::Path::FullFileName()
				API::String::UTF8 FullFileName(void) const final override;
				//! @copydoc CYB::API::Path::FileName()
				API::String::UTF8 FileName(void) const final override;
				//! @copydoc CYB::API::Path::Extension()
				API::String::UTF8 Extension(void) const final override;

				//! @copydoc CYB::API::Path::ByteLength()
				int ByteLength(void) const noexcept final override;

				void SetAsWorkingDirectory(void) const;

				/*!
					@brief Public access to the underlying string. Does not verify the path
					@return The UTF8 string representing the Path. Will always be fully evaluated
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				const API::String::UTF8& operator()(void) const noexcept;
			};
		};
	};
};