//! @file CYBSingleton.hpp Defines the engine core interface and static pointer as well as the Engine() access function
#pragma once

namespace CYB {
	namespace Engine {
		//! @brief An instance of this object is the entirety of the engine
		class Core {
		private:
			static Core* FCore;	//!< @brief Pointer to the single Core instance
			//Engine String	- Set FCore
			//Engine Version
			//Modules
				//User
				//GDI
				//Winsock
				//Vulkan
				//OpenAL
				//Optional
					//ExtendedKernel
					//XInput
			//Logger
				//Heap
			//Heap
			//Steam
			//API
				//
			//Unit
				//Load basics
				//Lock mutex
				//Heap
		private:
			/*!
				@brief Constructs the engine components
				@par Thread Safety
					This function should only be called once
				@par Exception Safety
					This function does not throw exceptions
			*/
			Core();
			/*!
				@brief Cleans up the engine and terminates the process
			*/
			~Core();
			/*!
				@brief Run the main unit
				@return true if the Unit should be launch again, false otherwise
				@par Thread Safety
					This function must be synchronized at the object level
				@par Exception Safety
					This function does not throw exceptions
			*/
			bool LaunchUnit(void);
		public:
			/*!
				@brief Retrieve the Core singleton
				@return A reference to the Core singleton
			*/
			static Core& GetCore(void);

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

	Engine::Core& Core(void);
};