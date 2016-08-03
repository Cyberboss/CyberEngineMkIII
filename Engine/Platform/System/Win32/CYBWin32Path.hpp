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
					public:
						using Constructor = API::Interop::Constructor<const System::Path&>;	//! @brief See @ref interstructors
					private:
						const System::Path& FOriginalPath;	//!< @brief The Path of the directory being enumerated
						API::Interop::Object<API::Path> FPathListing;	//!< @brief The API exposure for the current enumerated Path
						Win32::HANDLE FFindHandle;	//!< @brief The Win32 find handle
						Win32::WIN32_FIND_DATA FFindData;	//!< @brief The Win32 find data
					private:
						/*!
							@brief Assign the current data to FPathListing if it is not '.' or '..'
							@par Thread Safety
								This function thread safety at the object level
							@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
						*/
						void AssignOrRecurse(void);
					public:
						/*!
							@brief Begin the directory listing operation of @p APath
							@param APath The path whose contents we are listing
							@par Thread Safety
								This function requires no thread safety
							@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. If enumeration permission was denied
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
					Path() noexcept = default;	//!< @brief See @ref structors
				};
			};
		};
	};
};