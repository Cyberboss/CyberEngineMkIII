//! @file CYBCore.hpp Defines the engine core interface and static pointer as well as the Engine() access function
#pragma once

namespace CYB {
	namespace Engine {
		//! @brief An instance of this object is the entirety of the engine
		class Core : public API::Singleton<Core> {
		public:
			API::EngineInformation FEngineInformation;	//!< Information describing the engine

			Platform::Modules::ModuleManager FModuleManager;
				//User
				//GDI
				//Winsock
				//Vulkan
				//OpenAL
				//Optional
					//XInput
			//Logger
				//Heap
			Memory::Heap FHeap;
			//ThreadPool
			//Steam
			//Command Line
			//API
				//
			//Unit
				//Load basics
				//Lock mutex
				//Heap
		private:
			/*!
				@brief Constructs the engine components
				@param ANumArguments The number of command line arguments
				@param AArguments The command line arguments
				@par Thread Safety
					This function should only be called once
				@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::ErrorCode::MEMORY_RESERVATION_FAILURE. Thrown if a heap's memory could not be reserved
				@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::ErrorCode::MEMORY_COMMITAL_FAILURE. Thrown if a heap's memory could not be committed
			*/
			Core(const unsigned int ANumArguments, const oschar_t* const* const AArguments);
			//! @brief Cleans up the engine and terminates the process
			~Core() = default;

			/*!
				@brief Run the main unit
				@return true if the Unit should be launch again, false otherwise
				@par Thread Safety
					This function must be synchronized at the object level
			*/
			bool LaunchUnit(void) noexcept;
		public:
			/*!
				@brief Retrieve the Core singleton
				@return A reference to the Core singleton
				@par Thread Safety
					This function requires no thread safety
			*/
			static Core& GetCore(void) noexcept;

			/*!
				@brief Runs the engine and self terminates
				@param ANumArguments The number of command line arguments
				@param AArguments The command line arguments
				@par Thread Safety
					This function should only be called once
			*/
			static void Run(const unsigned int ANumArguments, const oschar_t* const* const AArguments) noexcept;
		};
	};

	//! @copydoc CYB::Engine::Core::GetCore
	Engine::Core& Core(void) noexcept;
};