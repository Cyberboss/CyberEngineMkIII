//! @file CYBPosixPath.hpp Defines CYB::Platform::System::Path for Posix
#pragma once

namespace CYB {
	namespace Platform {
		namespace System {
			class Path;
			namespace Implementation {
				//! @brief Implements directory iteration
				class Path {
				protected:
					//! @brief Uses the opendir API
					class DirectoryEntry : public API::Path::DirectoryEntry {
					public:
						//! @brief See @ref interstructors. See documented constructor CYB::Platform::System::Implementation::Path::DirectoryEntry::DirectoryEntry
						using Constructor = API::Interop::Constructor<const System::Path&>;
					private:
						const System::Path& FOriginalPath;	//!< @brief The Path of the directory being enumerated
						API::Interop::Object<API::Path> FPathListing;	//!< @brief The API exposure for the current enumerated Path
						Posix::DIR* FDirectory;	//!< @brief The directory entry
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
					/*!
						@brief Get the UTF8 string of the path of the executable image
						@return The UTF8 string of the path of the executable image
						@par Thread Safety
							This function requires no thread safet
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE If the path could not be retrieved
					*/
					static API::String::Dynamic GetExecutableImage(void);
				};
			};
		};
	};
};