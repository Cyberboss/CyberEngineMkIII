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
			protected:
				/*!
					@brief Constructor for self process. Use GetSelf()
					@par Thread Safety
						This function requires no thread safety
				*/
				Process() = default;
			public:
				/*!
					@brief Get's the Process representing the current execution
					@return The current Process
					@par Thread Safety
						This function requires no thread safety
				*/
				static Process GetSelf(void) noexcept;

				/*!
					@brief Launches a Process with a command line
					@param APath The path to the Process to launch
					@param ACommandLine The space delimited command line for the Process, quotes have no effect
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p APath does not exist in the filesystem
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p APath isn't accessible by the current user
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap ran out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the process could not be created
					@attention Launching processes which require administrator elevation on Windows will block the current thread until the authorization dialog is closed
				*/
				Process(const Path& APath, const API::String::UTF8& ACommandLine);
				/*!
					@brief Relaunches the current process with a new command line. Current process continues execution
					@param ACommandLine The space delimited command line for the Process, quotes have no effect
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p APath does not exist in the filesystem
					@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p APath isn't accessible by the current user
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap ran out of memory
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
					@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::SYSTEM_PATH_RETRIEVAL_FAILURE. Thrown if the path of the executable could not be retrieved
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the process could not be created
					@attention Launching processes which requires administrator elevation on Windows will block the current thread until the authorization dialog is closed
				*/
				Process(const API::String::UTF8& ACommandLine);
				//! @brief See @ref interstructors
				Process(const Process&) = delete;
				Process(Process&& AMove) noexcept = default;	//!< @brief See @ref structors
				Process& operator=(Process&& AMove) noexcept = default;	//!< @brief See @ref structors
				//! @brief Destroy the Process object without affecting the actual process
				~Process();

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
				/*!
					@brief Terminates the Process with exit code 0
					@par Thread Safety
						This function should only be called once per object instance
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_TERMINATION_ERROR if the process could not be terminated. Will not happen if called on current process
				*/
				void Terminate(void);

				/*!
					@brief Blocks execution until the associated Process is terminated or a timer expires
					@param AMilliseconds The number of milliseconds to wait before returning. Will wait indefinitely if set to zero
					@return true if the process has exited, false otherwise
					@par Thread Safety
						This function requires sychronization at the object level
				*/
				bool Wait(const unsigned int AMilliseconds = 0U);
				/*!
					@brief Blocks execution until the associated Process is terminated and returns it's exit code
					@return The Process' exit code
					@par Thread Safety
						If Active() is true, This function requires sychronization at the object level
					@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_EXIT_CODE_UNCHECKABLE Thrown if the OS would not allow the error code to be checked, usually due to a different user running the target process in the case of Windows elevation
				*/
				int GetExitCode(void);
			};
		};
	};
};
