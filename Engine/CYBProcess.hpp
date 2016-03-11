//! @file Process.hpp Defines the platform process interface
#pragma once

namespace CYB {
	//! @brief Encapsulates platform specific functions
	namespace Platform {
		//! @brief An object representing an operating system process
		class Process : private Implementation::Process {
		private:
			/*!
				@brief Runs actual termination code on a Process guaranteed to be destroyed afterwards
			*/
			void Terminate(void);
		public:
			using Implementation::Process::Process;
			/*!
				@brief Get's the Process representing the current execution
				@return The current Process
			*/
			static Process GetSelf(void);
			/*!
				@brief Terminates the Process with exit code zero and ensures destruction of the object
				@param AProcess An xvalue of the process to terminate
			*/
			static void Terminate(Process&& AProcess);
		};
	};
};

