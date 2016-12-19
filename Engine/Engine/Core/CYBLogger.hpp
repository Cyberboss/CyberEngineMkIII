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
			Memory::Heap FHeap;	//!< @brief The isolated Heap
			Context FContext;	//!< @brief The Context to be used when calling the Logger

			Platform::System::File FFile;	//!< @brief The file being written to
			Platform::System::Mutex FQueueLock,	//!< @brief The lock used to acquire access to FQueue
				FFileLock;	//!< @brief The lock used to acquire access to FFile
			
			LogEntry* FQueue;	//!< @brief The message queue
			API::Interop::Object<Platform::System::Thread> FThread;	//!< @brief The thread used for writing to the log file

			std::atomic_bool FCancelled;	//!< @brief Cancel flag for FThread
		private:
			/*!
				@brief Prepares the logging file for writing. May block for one millisecond if the preferred file name is taken in order to generate a new one
				@return The file to be used for logging
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if the log File could not be opened
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if the temporary Path could not be retrieved
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if Path errors occured while setting up the file
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_TOO_LONG. Thrown if the new Path would exceed the limitation
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if Path errors occured while setting up the file

			*/
			static Platform::System::File OpenFile(void);

			/*!
				@brief Empty FQueue and write it into FFile
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if the log File could not be written to
			*/
			void EmptyQueue(void);

			/*!
				@brief Writer thread. Runs in a loop until CancelThreadedOperation is called
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if the log File could not be written to
			*/
			void BeginThreadedOperation(void) override;
			/*!
				@brief Stops the writer thread
				@par Thread Safety
					This function requires no thread safety
			*/
			void CancelThreadedOperation(void) override;
		public:
			/*!
				@brief Initializes and starts the Logger. Changes the current Context. May block for one millisecond if the preferred file name is taken in order to generate a new one
				@param AEmergencyLogger The Logger to use when this one fails
				@par Thread Safety
					This function requires no thread safety
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_WRITABLE. Thrown if the log File could not be opened
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if the temporary Path could not be retrieved
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if Path errors occured while setting up the file
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_TOO_LONG. Thrown if the new Path would exceed the limitation
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::PATH_LOST. Thrown if Path errors occured while setting up the file
				@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::MUTEX_INITIALIZATION_FAILURE. Thrown if one of the Mutexes could not be initialized
				@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MEMORY_RESERVATION_FAILURE. Thrown if the Heap memory could not be reserved
				@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MEMORY_COMMITAL_FAILURE. Thrown if the Heap memory could not be committed
				@attention If this function does not throw a CYB::Exception::Internal then the current Context has been switched and should be reverted
			*/
			Logger(API::Logger& AEmergencyLogger);
			//! @brief Shutdown the Logger and empty the queue
			~Logger();

			//! @copydoc CYB::API::Logger::Log()
			void Log(const API::String::CStyle& AMessage, const Level ALevel) noexcept final override;

			//! @copydoc CYB::API::Logger::CurrentLog()
			const API::String::CStyle& CurrentLog(void) const noexcept final override;
		};
	};
};