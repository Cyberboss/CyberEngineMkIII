//! @file CYBProcess.hpp Defines the platform process interface
#pragma once

namespace CYB {
	//! @brief Encapsulates platform specific functions
	namespace Platform {
		namespace System {
			/*! 
				@brief An object representing an operating system process
				@par WARNING
					Processes with the same PID/Handle will appear equivalent. Ensure that a reference to a Process object does not persist through operating system recycling
			*/
			class Process : private Implementation::Process {
			private:
				using Implementation::Process::Process;
				/*!
					@brief Runs actual termination code on a Process guaranteed to be destroyed afterwards
					@par Thread Safety
						This function requires synchronized access at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				void Terminate(void) noexcept;
			public:
				/*!
					@brief Get's the Process representing the current execution
					@return The current Process
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static Process GetSelf(void) noexcept;
				/*!
					@brief Terminates the Process with exit code zero and ensures destruction of the object
					@param AProcess An xvalue of the process to terminate
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static void Terminate(Process&& AProcess) noexcept;

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
					@par Exception Safety
						This function does not throw exceptions
				*/
				bool Active(void) const noexcept;
				/*!
					@brief Check the equivalence of two Process objects
					@param ARHS Another Process to compare with
					@return true if the Processes are equivalent, false otherwise
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				bool operator==(const Process& ARHS) const noexcept;
			};
		};
	};
};
