#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			class Path;
			namespace Implementation {
				using namespace Posix;
				//! @brief Contains the PID and some wierd exit code retrival stuff
				class Process {
				protected:
					pid_t FPID;	//!< @brief The PID
					int FExitCode;	//!< @brief The exit code
					bool FExitCodeReady;	//!< @brief The status of FExitCode
				private:
					/*!
						@brief Spawn a process with the specified command line
						@param APath The path to the executable to spawn, can be an elevated executable
						@param ACommandLine The space delimited command line for the Process, quotes have no effect
						@return The PID for the spawned process
						@par Thread Safety
							This function requires no thread safety
						@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p APath does not exist in the filesystem
						@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p APath isn't accessible by the current user
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap ran out of memory
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
						@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the process could not be created
						@attention Launching processes which require administrator elevation on Windows will block the current thread until the authorization dialog is closed
					*/
					static pid_t SpawnProcess(const System::Path& APath, const API::String::Dynamic& ACommandLine);
					/*!
						@brief Called to throw an error based on errno after posix_spawn
						@par Thread Safety
							This function requires no thread safety
						@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if APath from a call to SpawnProcess  does not exist in the filesystem
						@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if APath from a call to SpawnProcess isn't accessible by the 
						@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the process from a call to SpawnProcess could not be created
					*/
					static void HandleSpawnError[[noreturn]](void);
				protected:
					/*!
						@brief Get's the Process representing the current execution
						@par Thread Safety
							This function requires no thread safety
					*/
					Process() noexcept;	
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
					*/
					Process(const System::Path& APath, const API::String::UTF8& ACommandLine);
					//! @brief See @ref structors
					Process(const Process&) = delete;
					//! @brief See @ref structors
					Process(Process&& AMove) noexcept = default;
					//! @brief See @ref structors
					Process& operator=(Process&& AMove) noexcept = default;
				};
			};
		};
	};
};