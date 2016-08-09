//! @file Path.hpp Defines the Path allocatable
#pragma once
namespace CYB {
	namespace API {
			/*!
				@brief Used for manipulating Paths. Paths will always exist either as a file or directory. Paths are '/' delimited when forming though may not be while retrieving. File names ".." will ascend a directory and '.' represents a no-op
				@attention Only UTF-8 encodedable paths are supported, paths lengths may not exceed MAX_PATH_BYTES, and directory names may not exceed 248 characters. Symlinks are always resolved on posix systems, but never on Windows systems. This is a user problem and should cause no errors so long as they do not reorganize the installation files. Note that the recursive folder creation and deletion options attempt to fully adhere to the strong guarantee. But, due to the nature of filesystem race conditions, this is impossible.
			*/
			class Path : public Interop::Allocatable {
			public:
				enum : int {
					MAX_PATH_BYTES = 256,
				};
				//! @brief Starting points for creating paths
				enum class SystemPath {
					EXECUTABLE_IMAGE,	//!< @brief The path from which the engine was launched, read only
					EXECUTABLE,	//!< @brief The directory from which the engine was launched, read only
					RESOURCE,	//!< @brief The directory where program resources are stored, read only
					TEMPORARY,	//!< @brief The directory for storing data relevant only to this execution
					USER,	//!< @brief The directory for storing permanent data associated with the current OS/Steam user, read write
					WORKING,	//!< @brief The current application working directory, permissions indeterminate
				};
				/*!
					@brief See @ref interstructors. Get the Path of a given SystemPath
					@par SystemPath
						The SystemPath to begin the path with
					@par Thread Safety
						Construction requires no thread safety
					@par Throws
						CYB::Exception::Violation Error code: <B>CYB::Exception::Violation::INVALID_ENUM</B>. If an invalid value for @p ADirectory is given
						<BR>CYB::Exception::SystemData Error code: <B>CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE</B>. Thrown if the current heap runs out of memory
						<BR>CYB::Exception::SystemData Error code: <B>CYB::Exception::SystemData::STRING_VALIDATION_FAILURE</B>. Thrown if the path string does not validate
						<BR>CYB::Exception::SystemData Error code: <B>CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE</B> if the specified path could not be retrieved
				*/
				using Constructor = Interop::Constructor<const SystemPath>;

				//! @brief Iterator for paths in a directory
				class DirectoryEntry : public Interop::Allocatable {
				public:
					/*!
						@brief Get the current path the iterator points to. Must be checked for validity after iteration. If invalid, end of iteration has been reached. Can be moved
						@return The current path the iterator points to
						@par Thread Safety
							Inherited from Path functions called. The reference is only valid until the iterator is advanced or destroyed
					*/
					virtual Interop::Object<Path>& operator*(void) noexcept = 0;
					/*!
						@brief Get the current path the iterator points to. Must be checked for validity after iteration. If invalid, end of iteration has been reached. Can be moved
						@return The current path the iterator points to
						@par Thread Safety
							Inherited from Path functions called. The reference is only valid until the iterator is advanced or destroyed
					*/
					virtual Interop::Object<Path>* operator->(void) noexcept = 0;

					/*!
						@brief Advance the iterator
						@par Thread Safety
							This function requires thread safety at the object level
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the string does not validate
					*/
					virtual void operator++(void) = 0;
				};

			public:
				/*!
					@brief Get the standardized UTF-8 directory separator string
					@return '/'
					@par Thread Safety
						This function requires no thread safety
				*/
				static const String::Static DirectorySeparatorChar(void) noexcept { return String::Static(u8"/"); }

				/*!
					@brief Append a directory/file to the path
					@param AAppendage The string to append to the path. This may contain '/'s and ".."s as required, though they should never start or end with '/'. If the last file in the path does not exist, the function will still succeed, however, further calls on the object will generate a CYB::Exception::SystemData::PATH_LOST error. This is to enable creation of new files
					@param ACreateIfNonExistant Create the end of path as a directory if it does not exist. If this is true then the last path token in @p AAppendage must not be ".."
					@param ACreateRecursive Create the path recursively. If this is true none of the path tokens in @p AAppendage should be "..", if one is it will be treated as unreadable. Ignored if @p ACreateIfNonExistant is false
					@attention This function will always fail if the current path is a file
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if a directory component of @p AAppendage could not be read
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if @p ACreateIfNonExistant is true and the new path could not be created. Filesystem state will be reverted even while doing recursive creation
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_TOO_LONG. Thrown if the new path would exceed the limitation
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if the current path failed to verify
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual void Append(const String::UTF8& AAppendage, const bool ACreateIfNonExistant, const bool ACreateRecursive) = 0;
				/*!
					@brief Navigate the path to the parent directory. Does NOT work on invalidated paths
					@attention On POSIX systems, symlinks in path names are resolved upon path evaluation. This could lead to strange behaviour, though the onus is on the user for changing their file system
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if access to parent directory is denied or the Path is currently the root directory.
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual void NavigateToParentDirectory(void) = 0;
				
				/*!
					@brief Ensures the current Path doesn't exist. This will invalidate the path
					@param ARecursive If true and IsDirectory() returns true, all files in the directory represented by this path will be deleted along with the directory
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::DIRECTORY_NOT_EMPTY. Thrown if the directory is not empty
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the file/directory could not be deleted
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual void Delete(bool ARecursive) = 0;

				/*!
					@brief Check if the current path is a directory
					@return true if the current path is a directory, false otherwise
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if a directory component of the path could not be read
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if the current path failed to verify
				*/
				virtual bool IsDirectory(void) const = 0;
				/*!
					@brief Check if the current path is a file
					@return true if the current path is a directory, false otherwise
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if a directory component of the path could not be read
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if the current path failed to verify
				*/
				virtual bool IsFile(void) const = 0;

				/*!
					@brief Get the name of the file without any prefixes. Equivalent to FileName and Extension if the name does not contain a period. Path can point to a file or directory
					@return The name of the file without any directory prefixes
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if a directory component of the path could not be read
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual String::UTF8 FullName(void) const = 0;
				/*!
					@brief Get the name of the file without any directory prefixes or extensions. Equivalent to FullFileName and Extension if the name does not contain a period. Path can point to a file or directory
					@return The name of the file without any directory prefixes or extensions
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual String::UTF8 Name(void) const = 0;
				/*!
					@brief Get the extension of the file without the leading period. Equivalent to FullFileName and FileName if the name does not contain a period. Path can point to a file or directory
					@return The extension of the file without the leading period
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if a directory component of the path could not be read
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual String::UTF8 Extension(void) const = 0;

				/*!
					@brief Get the length in bytes of the Path's underlying string
					@return The length in bytes of the Path's underlying string
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				virtual int ByteLength(void) const noexcept = 0;

				/*!
					@brief Get the first directory entry iterator in the path. Always valid
					@return The first directory entry iterator in the path
					@par Thread Safety
						This function requires synchronization at the object level
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if enumeration permission was denied
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if the current path failed to verify or is not a directory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
				*/
				virtual Interop::Object<DirectoryEntry> Contents(void) const = 0;
				
				/*!
					@brief Public access to the underlying string
					@return The UTF8 string representing the Path
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				virtual const String::UTF8& operator()(void) const noexcept = 0;
			};
	};
};
CYB_SET_ALLOCATABLE_ID(Path);