//! @file File.hpp Defines the API for files
namespace CYB {
	namespace API {
		//! @brief Contains the basic File interface. Does not perform locking of any kind, be aware of possible race conditions
		class File : public Interop::Allocatable {
		public:
			//! @brief The opening mode for the file
			enum class Mode {
				READ,	//!< @brief Read only mode
				WRITE,	//!< @brief Write only mode
				READ_WRITE,	//!< @brief Read write mode
			};
			//! @brief The method of handling Files that already exist
			enum class Method {
				ANY, //!< @brief The File may or may not exist. It will be created if it doesn't. OpenMethod will be set to EXIST or CREATE once constructed to indicate result
				EXIST,	//!< @brief The File must exist
				CREATE,	//!< @brief The File must not exist. It will be created
				TRUNCATE,	//!< @brief Any existing File will be overwritten and an empty file will be created
			};
			//! @brief The location to begin a seek operation at
			enum class SeekLocation {
				BEGIN,	//!< @brief Seek from the beginning of the file
				CURSOR,	//!< @brief Seek from the cursor position
				END,	//!< @brief Seek from the endo of the file
			};

			/*!
				@brief See @ref interstructors. Opens a file for access
				@par Path
					An xvalue of the Path of the File to open
				@par Mode 
					The Mode of opening the file, subsequent operations must respect this
				@par Method	
					The Method of handling preexisting files
				@par Thread Safety
					This function requires no thread safety
				@par Throws
					CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_READABLE. Thrown if @p AMode equals Mode::READ and the file could not be opened
					<BR>CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_WRITABLE. Thrown if @p AMode equals Mode::WRITE or Mode::READ_WRITE and the file could not be opened
					<BR>CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p AMethod equals Method::EXIST and the file portion of @p APath does not exist or if it is not valid in other cases.
					<BR>CYB::Exception::SystemData Error code: CYB::Exception::SystemData::FILE_EXISTS. Thrown if @p AMethod equals Method::CREATE and the file portion of @p APath already exists or is a directory
					<BR>CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap runs out of memory
					<BR>CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p AMode, or @p AMethod is invalid
					<BR>CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_PARAMETERS. Thrown if @p AMode is Mode::READ and @p AMethod is Method::TRUNCATE. Operating systems require write permissions to truncate a file
			*/
			using Constructor = Interop::Constructor<const Path&, const Mode, const Method>;
		public:
			/*!
				@brief Get the current size of the File
				@return The current size of the File
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual unsigned long long Size(void) const noexcept = 0;
			/*!
				@brief Get the current position of the read/write cursor in the File. Equivalent of Seek(0, SeekLocation::CURSOR)
				@return The current position of the read/write cursor in the File
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual unsigned long long CursorPosition(void) const noexcept = 0;

			/*!
				@brief Set the cursor position in the File
				@param AOffset The offset from ALocation to set the cursor to
				@param ALocation The initial location to begin the offset
				@return The current cursor position
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p ALocation is invalid
			*/
			virtual unsigned long long Seek(const long long AOffset, const SeekLocation ALocation) const = 0;

			/*!
				@brief Read data from a File at the current cursor position and advance the cursor by that amount
				@param[out] ABuffer The location to store the read data
				@param AMaxAmount The maximum number of bytes to be read
				@return The number of bytes read
				@par Thread Safety
					This function requres no thread safety
			*/
			virtual unsigned long long Read(void* const ABuffer, const unsigned long long AMaxAmount) const noexcept = 0;
			/*!
				@brief Write data to a File at the current cursor position and advance the cursor by that amount
				@param ABuffer The location to get the data to write
				@param AAmount The number of bytes to be written
				@return The number of bytes written
				@par Thread Safety
					This function requres no thread safety
			*/
			virtual unsigned long long  Write(const void* const ABuffer, const unsigned long long AAmount) noexcept = 0;

			/*!
				@brief Get the Path indicating the current File
				@return A reference to the Path indicating the current File
				@par Thread Safety
					This function requres no thread safety
			*/
			virtual const Path& Path(void) const noexcept = 0;

			/*!
				@brief Gets the opening method of the File. Will never be Method::ANY
				@return The opening method of the File. If it was orginally Method::ANY it will be changed to Method::EXIST or Method::CREATE
				@par Thread Safety
					This function requres no thread safety
			*/
			virtual Method OpenMethod(void) const noexcept = 0;
		};
	};
};
//! @cond
CYB_SET_ALLOCATABLE_ID(File);
//! @endcond