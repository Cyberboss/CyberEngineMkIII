//! @file CYBProcess.hpp Defines the platform process interface
#pragma once

namespace CYB {
	//! @brief Encapsulates platform specific functions
	namespace Platform {
		namespace System {
			class Path;
			/*! 
				@brief An object representing an operating system process
				@attention Processes with the same PID/Handle will appear equivalent. Ensure that a reference to a Process object does not persist through operating system recycling
			*/
			class Process : private Implementation::Process {
			private:
				using Implementation::Process::Process;
				/*!
					@brief Runs actual termination code on a Process guaranteed to be destroyed afterwards
					@par Thread Safety
						This function requires synchronized access at the object level
				*/
				void Terminate(void) noexcept;
			public:
				/*!
					@brief Get's the Process representing the current execution
					@return The current Process
					@par Thread Safety
						This function requires no thread safety
				*/
				static Process GetSelf(void) noexcept;
				/*!
					@brief Terminates the Process with exit code zero and ensures destruction of the object
					@param AProcess An xvalue of the process to terminate
					@par Thread Safety
						This function requires no thread safety
				*/
				static void Terminate(Process&& AProcess) noexcept;

				/*!
					@brief Launches a Process with a command line
					@param APath The path to the Process to launch
					@param ACommandLine The command line for the Process
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::ErrorCode::FILE_NOT_FOUND if @p APath does not exist in the filesystem
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::ErrorCode::FILE_NOT_READABLE if @p APath isn't accessible by the current user
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::ErrorCode::FILE_NOT_EXECUTABLE if @p APath does not indicate a process image
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::ErrorCode::PROCESS_CREATION_ERROR if the process could not be created
				*/
				Process(const Path& APath, const API::String::UTF8& ACommandLine);
				/*!
					@brief Relaunches the current process with a new command line. Current process continues execution
					@param ACommandLine The command line for the new instance
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::ErrorCode::FILE_NOT_FOUND if @p APath does not exist in the filesystem
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::ErrorCode::FILE_NOT_READABLE if @p APath isn't accessible by the current user
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::ErrorCode::SYSTEM_PATH_RETRIEVAL_FAILURE if the path of the executable could not be retrieved
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::ErrorCode::FILE_NOT_EXECUTABLE if @p APath does not indicate a process image
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::ErrorCode::PROCESS_CREATION_ERROR if the process could not be created
				*/
				Process(const API::String::UTF8& ACommandLine);
				Process(const Process&) = delete;
				Process(Process&& AMove) noexcept = default;	//!< @brief See @ref structors
				Process& operator=(Process&& AMove) noexcept = default;	//!< @brief See @ref structors
				//! @brief Destroy the Process object without affecting the actual process
				~Process() = default;

				/*!
					@brief Check if the Process is still running
					@return true if the Process is still running, false otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				bool Active(void) const noexcept;
				/*!
					@brief Check the equivalence of two Process objects
					@param ARHS Another Process to compare with
					@return true if the Processes are equivalent, false otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				bool operator==(const Process& ARHS) const noexcept;
				/*!
					@brief Check the unequivalence of two Process objects
					@param ARHS Another Process to compare with
					@return false if the Processes are equivalent, true otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				bool operator!=(const Process& ARHS) const noexcept;
			};
		};
	};
};
