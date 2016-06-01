//! @file CYBProcess.hpp Defines the platform process interface
#pragma once

namespace CYB {
	//! @brief Encapsulates platform specific functions
	namespace Platform {
		namespace System {
			//! @brief An object representing an operating system process
			class Process : private Implementation::Process {
			private:
				/*!
					@brief Runs actual termination code on a Process guaranteed to be destroyed afterwards
					@par Thread Safety
						This function requires synchronized access at the object level
					@par Exception Safety
						This function does not throw exceptions
				*/
				void Terminate(void);
			protected:
				using Implementation::Process::Process;
			public:
				Process(const Process&) = delete;
				Process(Process&& AMove);	//!< @brief See @ref structors
				Process& operator=(Process&& AMove);	//!< @brief See @ref structors
				//! @brief Closes the handle to a process
				~Process() = default;

				/*!
					@brief Get's the Process representing the current execution
					@return The current Process
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static Process GetSelf(void);
				/*!
					@brief Terminates the Process with exit code zero and ensures destruction of the object
					@param AProcess An xvalue of the process to terminate
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static void Terminate(Process&& AProcess);
			};
		};
	};
};
