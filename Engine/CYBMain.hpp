//! @file CYBMain.hpp Defines engine initialization functions

#pragma once

//! @brief Encapsulates the engine
namespace CYB {
	//! @brief Encapsulates platform agnostic engine functions
	namespace Engine {
		//! @brief Contains engine initialization functions
		class Init {
		public:
			//! @brief Runs the engine and self terminates
			static void Run[[noreturn]](void);
		};
	};
};