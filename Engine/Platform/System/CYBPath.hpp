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
			public:
				typedef API::Interop::Constructor<API::String::UTF8&&> Constructor;
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
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if the specified path could not be retrieved
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				static API::String::UTF8 LocateDirectory(const SystemPath ADirectory);
				/*!
					@brief Get the string path of SystemDirectory::RESOURCE
					@return The string path of SystemDirectory::RESOURCE
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if an invalid value for @p ADirectory is given
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if the specified path could not be retrieved
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				static API::String::UTF8 GetResourceDirectory(void);
				
				/*!
					@brief Ensure the existance of a single directory. Path's before it must exist
					@param APath The path to the directory that will be created. Last token must not be ".."
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the directory could not be created
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
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
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				static void CreateDirectories(const API::String::UTF8& AExistingPath, const API::Container::Deque<API::String::UTF8>& APaths);
				
				/*!
					@brief Ensure the non-existance of a single file. Path's before it must exist. Directory must be empty
					@param APath The path to the file that will be deleted
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the directory could not be deleted
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
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
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				static void DeleteDirectory(const API::String::UTF8& APath);

				/*!
					@brief Get the index of the last of a given seperator in a string
					@param AString The path to get the index of the last seperator
					@param ASeparator The separator to look for
					@return The index of the last @p ASeparator in @p AString or -1 if it was not found
					@par Thread Safety
						This function requires no thread safety
				*/
				static int GetIndexOfLastSeperator(const API::String::UTF8& AString, const char ASeparator) noexcept;

				/*!
					@brief Evaluates '..' and '.' references within the path
					@param[out] APath The path to evaluate
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::PATH_EVALUATION_FAILURE. Thrown if the path could not be evaluated
				*/
				static void Evaluate(API::String::UTF8& APath);

				/*!
					@brief Verifys the path pointed to exists
					@param APath The path to check
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if some part of the new path is not readable with the current permissions
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				bool Verify(const API::String::UTF8& APath) const;

				/*!
					@brief Sets the current Path string
					@param APath The string to set the path to
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				void SetPath(API::String::UTF8&& APath);
			public:

				/*!
					@brief Sets the path, must be valid 
					@param APath An xvalue of the path
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				Path(API::String::UTF8&& APath);
				/*!
					@brief Get the Path of a given SystemPath
					@param ADirectory The type of SystemPath to look up
					@par Thread Safety
						Use of CYB::Platform::Path::SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if an invalid value for @p ADirectory is given
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if the specified path could not be retrieved
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				Path(const SystemPath ADirectory);
				/*!
					@brief See @ref structors
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
				*/
				Path(const Path& ACopy) = default;
				Path(Path&& AMove) noexcept = default;	//! @brief See @ref structors
				Path& operator=(Path&& AMove) noexcept = default;	//! @brief See @ref structors
				~Path() final override = default;	//! @brief See @ref structors

				void SetAsWorkingDirectory(void) const;

				/*!
					@brief Public access to the underlying string. Does not verify the path
					@return The UTF8 string representing the Path. Will always be fully evaluated
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				const API::String::UTF8& operator()(void) const noexcept;

				//! @copydoc CYB::API::Path::Append()
				void Append(const API::String::UTF8& AAppendage, const bool ACreateIfNonExistant, const bool ACreateRecursive) final override;
				//! @copydoc CYB::API::Path::NavigateToParentDirectory()
				void NavigateToParentDirectory(void) final override;
				
				//! @copydoc CYB::API::Path::Delete()
				void Delete(bool ARecursive) const final override;

				//! @copydoc CYB::API::Path::IsDirectory()
				bool IsDirectory(void) const final override;
				//! @copydoc CYB::API::Path::IsFile()
				bool IsFile(void) const final override;

				//! @copydoc CYB::API::Path::FullName()
				API::String::UTF8 FullName(void) const final override;
				//! @copydoc CYB::API::Path::Name()
				API::String::UTF8 Name(void) const final override;
				//! @copydoc CYB::API::Path::Extension()
				API::String::UTF8 Extension(void) const final override;

				//! @copydoc CYB::API::Path::ByteLength()
				int ByteLength(void) const noexcept final override;
				//! @copydoc CYB::API::Path::Contents()
				API::Interop::Object<API::Path::DirectoryEntry> Contents(void) const final override;
			};
		};
	};
};