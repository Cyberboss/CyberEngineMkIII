//! @file CYBMain.hpp Defines engine initialization functions

#pragma once

namespace CYB {
	//! @brief Encapsulates platform agnostic engine functions
	namespace Engine {
		//! @brief Contains engine initialization functions
		class Init {
		public:
			/*! 
				@brief Runs the engine and self terminates
				@par Thread Safety
					This function should only be called once
				@par Exception Safety
					This function does not throw exceptions
			*/
			static void Run[[noreturn]](void);
		};
	};
};