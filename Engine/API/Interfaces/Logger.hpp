//! @file Logger.hpp Defines CYB::API::Logger
#pragma once

namespace CYB {
	namespace API {
		class Path;
		//! @brief The interface for writing safe logs
		class Logger {
		public:
			//! @brief The severity of the log
			enum class Level : byte {
				DEV,	//!< @brief Debug messages
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
				In the near impossible case that the isolated Heap the Logger uses runs out of memory, this function will block and flush the write queue to free space
			*/
			virtual void Log(const String::CStyle& AMessage, const Level ALevel) noexcept = 0;

			/*!
				@brief Retrieve the Path of the File the Logger is currently writing to
				@return The Path of the File the Logger is currently writing to
				@par Thread Safety
					This function requires no thread safety
			*/
			virtual const Path& CurrentLog(void) const noexcept = 0;
		};
	};
};