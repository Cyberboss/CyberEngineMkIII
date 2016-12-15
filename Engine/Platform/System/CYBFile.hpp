//! @file CYBFile.hpp Defines the filesystem interface
namespace CYB {
	namespace Platform {
		namespace System {
			//! @brief Contains the basic File interface. Does not perform locking of any kind, be aware of possible race conditions
			class File : private Implementation::File, public API::File {
			public:
				System::Path FPath;	//!< @brief The Path indicating this File
				Mode FOpenMode;	//!< @brief The method used to open the file
				Method FOpenMethod;	//!< @brief The method used to open the file
			private:
				/*!
					@brief Closes the file's handle if it is valid
					@par Thread Safety
						This function requires no thread safety
				*/
				void Close(void) const noexcept;
			public:
				/*!
					@brief Equivalent to the statement File(APath, Mode::WRITE, Method::ANY);. Opens/creates a File and updates its access times
					@param APath The Path of the File to open
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if the file could not be opened for writing
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p APath is not valid.
				*/
				static void Touch(System::Path&& APath);
				/*!
					@brief Get the current size of a File
					@param APath The Path of the File to open
					@return The current size of the File indicated by @p APath
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if the file size could not be retrieved
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p APath failed to verify or the file does not exist
				*/
				static unsigned long long Size(const System::Path& APath);

				/*!
					@brief Opens a file for access
					@param APath The Path of the File to open
					@param AMode The Mode of opening the file, subsequent operations must respect this
					@param AMethod The Method of handling preexisting files
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p AMode equals Mode::READ and the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if @p AMode equals Mode::WRITE or Mode::READ_WRITE and the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p AMethod equals Method::EXIST and the file portion of @p APath does not exist or if it is not valid in other cases
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_EXISTS. Thrown if @p AMethod equals Method::CREATE and the file portion of @p APath already exists or is a directory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p AMode, or @p AMethod is invalid
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_PARAMETERS. Thrown if @p AMode is Mode::READ and @p AMethod is Method::TRUNCATE. Operating systems require write permissions to truncate a file
				*/
				File(const API::Path& APath, const Mode AMode, const Method AMethod);
				/*!
					@brief Opens a file for access
					@param APath The Path of the File to open
					@param AMode The Mode of opening the file, subsequent operations must respect this
					@param AMethod The Method of handling preexisting files
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p AMode equals Mode::READ and the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if @p AMode equals Mode::WRITE or Mode::READ_WRITE and the file could not be opened
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p AMethod equals Method::EXIST and the file portion of @p APath does not exist or if it is not valid in other cases
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_EXISTS. Thrown if @p AMethod equals Method::CREATE and the file portion of @p APath already exists or is a directory
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p AMode, or @p AMethod is invalid
					@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_PARAMETERS. Thrown if @p AMode is Mode::READ and @p AMethod is Method::TRUNCATE. Operating systems require write permissions to truncate a file
				*/
				File(CYB::Platform::System::Path&& APath, const CYB::API::File::Mode AMode, const CYB::API::File::Method AMethod);
				//! @brief See @ref structors
				File(const File&) = delete;
				//! @brief See @ref structors
				File(File&& AMove) noexcept = default;
				//! @brief See @ref structors
				File& operator=(File&& AMove) noexcept;
				//! @brief Closes a file, saving changes
				~File() final override;

				//! @copydoc CYB::API::Stream::Capabilities()
				void Capabilities(Mode& AMode, bool& ASeek) const noexcept final override;

				//! @copydoc CYB::API::File::Size()
				unsigned long long Size(void) const final override;
				//! @copydoc CYB::API::File::CursorPosition()
				unsigned long long CursorPosition(void) const noexcept final override;
				//! @copydoc CYB::API::Stream::Seek()
				unsigned long long Seek(const long long AOffset, const SeekLocation ALocation) const final override;

				//! @copydoc CYB::API::Stream::Read()
				unsigned long long Read(void* const ABuffer, const unsigned long long AMaxAmount) const final override;
				//! @copydoc CYB::API::Stream::Write()
				unsigned long long Write(const void* const ABuffer, const unsigned long long AAmount) final override;

				//! @copydoc CYB::API::File::GetPath()
				const API::Path& GetPath(void) const noexcept final override;
				//! @copydoc CYB::API::File::OpenMethod();
				Method OpenMethod(void) const noexcept final override;
			};
		};
	};
};