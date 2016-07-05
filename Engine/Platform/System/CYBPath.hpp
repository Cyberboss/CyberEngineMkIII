//! @file CYBPath.hpp Defines the Path interface
#pragma once
namespace CYB {
	namespace Platform {
		namespace System {
			/*!
				@brief Used for manipulating Paths. Paths will always exist either as a file or directory. Paths are '/' delimited when forming though may not be while retrieving. File names ".." will ascend a directory and '.' represents a no-op
				@attention Only UTF-8 encodedable paths are supported, paths lengths may not exceed 256 BYTES, and directory names may not exceed 248 characters
			*/
			class Path {
			public:
				enum class SystemPath {
					EXECUTABLE_IMAGE,	//!< @brief The path from which the engine was launched
					EXECUTABLE,	//!< @brief The directory from which the engine was launched
					RESOURCE,	//!< @brief The directory where read only resources
					TEMPORARY,	//!< @brief The directory for storing data relevant only to this execution
					USER,	//!< @brief The directory for storing permanent data associated with the OS' current user
					WORKING,	//!< @brief The current application working directory
				};
			private:
				API::String::UTF8 FPath;	//!< @brief The underlying string
			private:
				/*!
					@brief Get the string path of a SystemDirectory
					@param ADirectory The type of SystemDirectory to look up
					@return The string path of @p ADirectory
					@par Thread Safety
						Use of SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
				*/
				static API::String::UTF8 LocateDirectory(const SystemPath ADirectory);

				bool Verify(void) const;
				void Evaluate(void);
			public:
				/*!
					@brief Get the Path of a SystemDirectory
					@param ADirectory The type of SystemDirectory to look up
					@par Thread Safety
						Use of CYB::Platform::Path::SystemDirectory::WORKING must be synchronized with SetAsWorkingDirectory. Otherwise, this function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the specified path could not be retrieved
				*/
				Path(const SystemPath ADirectory);
				Path(API::String::UTF8&& AString, const bool ACreateIfNonExistant, const bool ACreateRecursive);
				Path(const Path& ACopy) = default;
				Path(Path&& AMove) noexcept = default;
				Path& operator=(Path&& AMove) noexcept = default;

				/*!
					@brief Append a directory/file to the path
					@param AAppendage The string to append to the path. This may contain '/'s and ".."s as required
					@param ACreateIfNonExistant Create the path as a directory if it does not exist
					@param ACreateRecursive Ignored if @p ACreateIfNonExistant is false
					@return true if navigation succeeded, false otherwise
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE If some part of the new path is not readable. This does NOT include the final file
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE If the new path could not be created. Filesystem state will be reverted even while doing recursive creation
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_TOO_LONG If the new path would exceed the limitation
				*/
				bool Append(const API::String::UTF8& AAppendage, const bool ACreateIfNonExistant, const bool ACreateRecursive);
				void Delete(const bool ARecursive) const;

				bool IsDirectory(void) const;
				bool IsFile(void) const;

				bool CanRead(void) const;
				bool CanWrite(void) const;
				bool CanExecute(void) const;

				void SetAsWorkingDirectory(void) const;

				Path operator+(const Path& AOtherPath) const;
				/*!
					@brief Public access to the underlying string
					@return The UTF8 string representing the Path. Will always be fully evaluated
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				const API::String::UTF8& operator()(void) const noexcept;

				int ByteLength(void) const;
			};
		};
	};
};