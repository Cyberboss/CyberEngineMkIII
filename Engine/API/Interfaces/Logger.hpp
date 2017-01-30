//! @file Logger.hpp Defines CYB::API::Logger
#pragma once

namespace CYB {
	namespace API {
		//! @brief The interface for writing safe logs
		class Logger {
		public:
			//! @brief The severity of the log
			enum class Level : byte {
				DEV,	//!< @brief Debug messages, enabled/disabled by default in engine debug/release builds respectively
				INFO,	//!< @brief Generic information
				WARN,	//!< @brief Recoverable warnings
				ERR,	//!< @brief Hard errors
			};
		public:
			/*!
				@brief Log a message. Will be written to a text file on the Path returned by CurrentLog
				@param AMessage The message to log
				@param ALevel The Level of the message
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::Violation Error code: CYB::Exception::Violation::INVALID_ENUM. Thrown if the @p ALevel is invalid
				In the near impossible case that the isolated Heap the Logger uses runs out of memory, this function will block and flush the write queue to free space
				This function may silently fail in the case that the log cannot be written to
			*/
			virtual void Log(const String::CStyle& AMessage, const Level ALevel) = 0;

			/*!
				@brief Delays execution until all pending logs from the current thread have been written to the output
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void Flush(void) const noexcept = 0;

			/*!
				@brief Retrieve the string representation of the Path of the File the Logger is currently writing to
				@return The string representation of the Path of the File the Logger is currently writing to
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual const String::CStyle& CurrentLog(void) const noexcept = 0;

			/*!
				@brief Enable/Disable filtering of Level::DEV logs
				@param AEnable Enables DEV logs if true, disables them otherwise
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual void SetDebugLogging(const bool AEnable) noexcept = 0;
		};
	};
};