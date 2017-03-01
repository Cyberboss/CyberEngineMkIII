//! @file CYBWin32Process.hpp Defines the Win32 implementation for CYB::Platform::System::Process
#pragma once
namespace CYB {
	namespace Platform {
		namespace System{
			class Path;
			namespace Implementation {
				//! @brief Contains the Process handle and spawning function
				class Process {
				public:
					Win32::HANDLE FHandle;	//!< @brief The Process handle
				private:
					/*!
						@brief Spawn a process with the specified command line
						@param APath The path to the executable to spawn, can be an elevated executable
						@param ACommandLine The space delimited command line for the Process, quotes have no effect
						@return The handle for the spawned process
						@par Thread Safety
							This function requires no thread safety
						@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::FILE_NOT_FOUND. Thrown if @p APath does not exist in the filesystem
						@throws CYB::Exception::SystemData Error Code: CYB::Exception::SystemData::STREAM_NOT_READABLE. Thrown if @p APath isn't accessible by the current user
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::HEAP_ALLOCATION_FAILURE. Thrown if the current heap ran out of memory
						@throws CYB::Exception::SystemData Error code: CYB::Exception::SystemData::STRING_VALIDATION_FAILURE. Thrown if the path string does not validate
						@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the process could not be created
						@attention Launching processes which require administrator elevation on Windows will block the current thread until the authorization dialog is closed
					*/
					static Win32::HANDLE CreateProcess(const System::Path& APath, const API::String::UTF8& ACommandLine);

					/*!
						@brief Create and verify a handle from a given string
						@param ATextHandle The numeric text for the handle
						@par Thread Safety
							This function requires no thread safety
						@return A valid handle from @p ATextHandle
						@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the handle was invalid in some form
					*/
					static Win32::HANDLE HexToHandle(const API::String::CStyle& ATextHandle);
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
						@attention Launching processes which requires administrator elevation will block the current thread until the authorization dialog is closed
					*/
					Process(const System::Path& APath, const API::String::UTF8& ACommandLine);
					//! @brief See @ref structors
					Process(const Process&) = delete;
					//! @brief See @ref structors
					Process(Process&& AMove) noexcept;
					//! @brief See @ref structors
					Process& operator=(Process&& AMove) noexcept;
				public:
					/*!
						@brief Attempt to inherit a handle from a given string
						@param ATextHandle The numeric text for the handle
						@par Thread Safety
							This function requires no thread safety
						@throws CYB::Exception::Internal Error Code: CYB::Exception::Internal::PROCESS_CREATION_ERROR. Thrown if the handle was invalid in some form
					*/
					Process(const API::String::CStyle& ATextHandle);
				};
			};
		};
	};
};