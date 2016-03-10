//! @file Process.hpp Defines the platform process interface
#pragma once

namespace CYB {
	//! @brief Encapsulates platform specific functions
	namespace Platform {
		//! @brief An object representing an operating system process
		class Process : private Implementation::Process {
		public:
			using Implementation::Process::Process;
			/*!
				@brief Terminates the Process
			*/
			void Terminate(void);
			/*!
				@brief Get's the Process representing the current execution
				@return The current Process
			*/
			static Process GetSelf(void);
		};
	};
};

