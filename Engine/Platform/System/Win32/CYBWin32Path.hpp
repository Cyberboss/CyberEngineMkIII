//! @file CYBWin32Path.hpp Defines the Win32 implementation for CYB::Platform::System::Path
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			class Path;
			namespace Implementation {
				//! @brief Adds a UTF16 accompaniment to the UTF8 string of System::Path
				class Path {
				protected:
					//! @brief Uses the FindFile API to enumerate directories
					class DirectoryEntry : public API::Path::DirectoryEntry {
					private:
						const System::Path& FOriginalPath;	//!< @brief The Path of the directory being enumerated
						API::Interop::Object<API::Path> FPathListing;	//!< @brief The API exposure for the current enumerated Path
						Win32::HANDLE FFindHandle;	//!< @brief The Win32 find handle
					private:
						/*!
							@brief Assign the current data to FPathListing if it is not '.' or '..'
							@param AFindData The current find data
							@par Thread Safety
								This function thread safety at the object level
							@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
						*/
						void AssignOrRecurse(Win32::WIN32_FIND_DATA&& AFindData);
					public:
						/*!
							@brief Begin the directory listing operation of @p APath
							@param APath The path whose contents we are listing
							@par Thread Safety
								This function requires no thread safety
							@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. If enumeration permission was denied
							@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
							@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST If the current path failed to verify
						*/
						DirectoryEntry(const System::Path& APath);
						//! @brief See @ref structors
						~DirectoryEntry() final override;

						//! @copydoc CYB::API::Path::DirectoryEntry::operator*()
						API::Interop::Object<API::Path>& operator*(void) noexcept final override;
						//! @copydoc CYB::API::Path::DirectoryEntry::operator->()
						API::Interop::Object<API::Path>* operator->(void) noexcept final override;

						//! @copydoc CYB::API::Path::DirectoryEntry::operator++()
						void operator++(void) final override;
					};
				protected:
					API::String::UTF16 FWidePath;	//!< @brief The UTF16 string
				protected:
					/*!
						@brief Try to check for the existence of a directory
						@param APath The path of the directory to check
						@return -1 if an error occured. 0 if @p APath is not a directory. 1 if @p APath is a directory
						@par Thread Safety
							This function requires no thread safety
					*/
					static int CheckDirectoryExists(const API::String::UTF16& APath) noexcept;

					Path() noexcept = default;	//!< @brief See @ref structors
				public:
					/*!
						@brief Accessor for other Win32 implementations that require the UTF16 Path
						@return A reference to the UTF16 string of the Path
						@par Thread Safety
							This function requires synchronization at the object level
					*/
					const API::String::UTF16& WidePath(void) const noexcept;
				};
			};
		};
	};
};