//! @file CYBLogger.hpp Defines CYB::Engine::Logger
#pragma once

namespace CYB {
	namespace Engine {
		//! @brief The threaded, queued engine logger
		class Logger : public API::Logger, private API::Threadable {
		private:
			//! @brief Log entries to be inserted into the queue
			struct LogEntry {
				LogEntry* FNext;	//!< @brief Next item in the linked list
				API::String::Dynamic FMessage;	//!< @brief The pre-formatted text of the log
				Level FLevel;	//!< @brief The Level of the message
			};
		private:
			Platform::System::File FFile;	//!< @brief The file being written to
			Platform::System::Mutex FLock,	//!< @brief The lock used to acquire access to FQueue
				FFileLock;	//!< @brief The lock used to acquire access to FFile

			Memory::Heap FHeap;	//!< @brief The isolated Heap
			
			LogEntry* FQueue;	//!< @brief The message queue

			Platform::System::Thread FThread;	//!< @brief The thread used for writing to the log file
			std::atomic_bool FCancelled;	//!< @brief Cancel flag for FThread
		private:
			/*!
				@brief Empty FQueue and write it into FFile
				@par Thread Safety
					This function requires no thread safety
			*/
			void EmptyQueue(void) noexcept;

			/*!
				@brief Writer thread. Runs in a loop until CancelThreadedOperation is called
				@par Thread Safety
					This function requires no thread safety
			*/
			void BeginThreadedOperation(void) override;
			/*!
				@brief Stops the writer thread
				@par Thread Safety
					This function requires no thread safety
			*/
			void CancelThreadedOperation(void) override;
		public:
			//! @copydoc CYB::API::Logger::Log()
			void Log(const API::String::CStyle& AMessage, const Level ALevel) noexcept final override;

			//! @copydoc CYB::API::Logger::CurrentLog()
			const API::Path& CurrentLog(void) const noexcept final override;
		};
	};
};