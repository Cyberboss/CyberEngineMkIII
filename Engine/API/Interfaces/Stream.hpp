//! @file Stream.hpp Defines CYB::API::Stream Interface
#pragma once

namespace CYB {
	namespace API {
		//! @brief The interface for streaming reads and writes
		class Stream {
		public:
			//! @brief The available operations for the Stream. Bitset
			enum class Mode : byte {
				READ = 1,	//!< @brief Read only mode
				WRITE,	//!< @brief Write only mode
				READ_WRITE,	//!< @brief Read write mode
			};
			//! @brief The location to begin a seek operation at
			enum class SeekLocation {
				BEGIN,	//!< @brief Seek from the beginning of the Stream
				CURSOR,	//!< @brief Seek from the cursor position
				END,	//!< @brief Seek from the end of the Stream
			};
		public:
			/*!
				@brief Get the capabilities of the stream
				@param[out] AMode Returns the Mode of the Stream
				@param[out] ASeek Returns true if the stream is seekable, false otherwise
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void Capabilities(Mode& AMode, bool& ASeek) const noexcept = 0;

			/*!
				@brief Get the current position of the read/write cursor in the Stream. Equivalent of Seek(0, SeekLocation::CURSOR)
				@return The current position of the cursor in the Stream
				@par Thread Safety
					This function requires synchronization at the object level
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_OPERATION. Thrown if the Stream cannot be seeked
			*/
			virtual unsigned long long CursorPosition(void) const = 0;

			/*!
				@brief Set the cursor position in the Stream
				@param AOffset The offset from ALocation to set the cursor to
				@param ALocation The initial location to begin the offset
				@return The current cursor position
				@par Thread Safety
					This function requires synchronization at the object level
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if an error occurs during the seek
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if @p ALocation is invalid
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_OPERATION. Thrown if the Stream cannot be seeked
			*/
			virtual unsigned long long Seek(const long long AOffset, const SeekLocation ALocation) const = 0;

			/*!
				@brief Read data from a Stream at the current cursor position and advance the cursor by that amount
				@param[out] ABuffer The location to store the read data
				@param AMaxAmount The maximum number of bytes to be read
				@return The number of bytes read. If zero is returned and the cursor position is not SeekLocation::END an error has occurred. If the return value does not equal @p AMaxAmount, assume the underlying medium is currently out of readable data
				@par Thread Safety
					This function requires synchronization at the object level
				@throw CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_OPERATION. Thrown if the Stream mode is Mode::WRITE
			*/
			virtual unsigned long long Read(void* const ABuffer, const unsigned long long AMaxAmount) const = 0;
			/*!
				@brief Write data to the Stream at the current cursor position and advance the cursor by that amount
				@param ABuffer The location to get the data to write
				@param AAmount The number of bytes to be written
				@return The number of bytes written. If zero is returned, an error has occurred. If the return value does not equal @p AMaxAmount, assume the underlying medium is currently out of space
				@par Thread Safety
					This function requires synchronization at the object level
				@throw CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_OPERATION. Thrown if the Stream mode is Mode::READ
			*/
			virtual unsigned long long Write(const void* const ABuffer, const unsigned long long AAmount) = 0;
		};

		/*!
			@brief A Stream guaranteed to not throw exceptions
			@attention As a result, invalid operations will result in calls to HCF
		*/
		class SafeStream : public Stream {
		public:
			/*!
				@brief Get the current position of the read/write cursor in the Stream. Equivalent of Seek(0, SeekLocation::CURSOR)
				@return The current position of the cursor in the Stream
				@par Thread Safety
					This function requires synchronization at the object level
			*/
			virtual unsigned long long CursorPosition(void) const noexcept = 0;
			/*!
				@brief Set the cursor position in the Stream
				@param AOffset The offset from ALocation to set the cursor to
				@param ALocation The initial location to begin the offset
				@return The current cursor position
				@par Thread Safety
					This function requires synchronization at the object level
			*/
			virtual unsigned long long Seek(const long long AOffset, const SeekLocation ALocation) const noexcept = 0;

			/*!
				@brief Read data from a Stream at the current cursor position and advance the cursor by that amount
				@param[out] ABuffer The location to store the read data
				@param AMaxAmount The maximum number of bytes to be read
				@return The number of bytes read. If zero is returned, and the cursor position is not as SeekLocation::END an error has occurred and the read should not be retried
				@par Thread Safety
					This function requires synchronization at the object level
			*/

			virtual unsigned long long Read(void* const ABuffer, const unsigned long long AMaxAmount) const noexcept = 0;
			/*!
				@brief Write data to the Stream at the current cursor position and advance the cursor by that amount
				@param ABuffer The location to get the data to write
				@param AAmount The number of bytes to be written
				@return The number of bytes written. If zero is returned, an error has occurred and the write should not be retried
				@par Thread Safety
					This function requires synchronization at the object level
			*/
			virtual unsigned long long Write(const void* const ABuffer, const unsigned long long AAmount) noexcept = 0;
		};
	};
};