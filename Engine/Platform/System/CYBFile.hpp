//! @file CYBFile.hpp Defines the filesystem interface
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Contains the basic File interface. Does not perform locking of any kind, be aware of possible race conditions
			class File : public API::File {
			public:
				const System::Path FPath;	//!< @brief The Path indicating this File
			public:
				/*!
					@brief Equivalent to the statement File(APath, Mode::WRITE, Method::ANY);. Opens/creates a File and updates its access times
					@param APath The Path of the File to open
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if @p APath failed to verify
				*/
				static void Touch(const System::Path& APath);
				/*!
					@brief Get the current size of a File
					@param APath The Path of the File to open
					@return The current size of the File indicated by @p APath
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if the file size could not be retrieved
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if @p APath failed to verify
				*/
				static unsigned long long Size(const System::Path& APath);
				
				/*!
					@brief Opens a file for access
					@param APath The Path of the File to open
					@param AMode The Mode of opening the file, subsequent operations must respect this
					@param AMethod The Method of handling preexisting files
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if @p AMode equals Mode::READ and the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if @p AMode equals Mode::WRITE or Mode::READ_WRITE and the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if @p APath failed to verify
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p AMode, or @p AMethod is invalid
				*/
				File(const API::Path& APath, const Mode AMode, const Method AMethod);
				//! @copydoc CYB::Platform::System::File::File(CYB::API::Path&, CYB::API::File::Mode, CYB::API::File::Method)
				File(System::Path&& APath, const Mode AMode, const Method AMethod);
				File(const File&) = delete;
				//! @brief See @ref structors
				File(File&& AMove) noexcept;
				//! @brief See @ref structors
				File& operator=(File&& AMove) noexcept = default;

				//! @copydoc CYB::API::File::Size()
				unsigned long long Size(void) const noexcept final override;
				//! @copydoc CYB::API::File::CursorPosition()
				unsigned long long CursorPosition(void) const noexcept final override;

				//! @copydoc CYB::API::File::Seek()
				void Seek(const long long AOffset, const SeekLocation ALocation) const final override;

				//! @copydoc CYB::API::File::Read()
				unsigned long long Read(void* const ABuffer, const unsigned long long AMaxAmount) const final override;
				//! @copydoc CYB::API::File::Write()
				void Write(void* const ABuffer, const unsigned long long AAmount) final override;

				//! @copydoc CYB::API::File::Path()
				const API::Path& Path(void) const noexcept final override;
			};
		};
	};
};
//! @todo Use File::Touch in Path tests after it's implemented