//! @file CYBModuleManager.hpp Contains the declaration of CYB::Platform::Manager
#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			//! @brief Encapsulates the engine's loaded modules
			class Manager {
				ENABLE_TEST_HOOKS
			private:
				/*!
					@brief Get a pointer to the requested AutoModule
					@tparam AAutoModule The AutoModule to retrieve
					@return A pointer to the requested AutoModule if loaded, nullptr otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				template <typename AAutoModule> auto GetAutoModule(void) noexcept -> AAutoModule*;
				/*!
					@brief Load the requested optional AutoModule. Should be alternated with calls to UnloadAutoModule<AAutoModule>
					@tparam AAutoModule The AutoModule to load
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				template <typename AAutoModule> void LoadAutoModule(void) noexcept;
				/*!
					@brief Unoad the requested optional AutoModule. Should be alternated with calls to LoadAutoModule<AAutoModule>
					@tparam AAutoModule The AutoModule to unload
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				template <typename AAutoModule> void UnloadAutoModule(void) noexcept;
			private:
				//! @cond
				//Windows
				REQURIED_MODULE_FIELD(Kernel32);
				REQURIED_MODULE_FIELD(Kernel32Extended);
				REQURIED_MODULE_FIELD(Shell);
				REQURIED_MODULE_FIELD(Ole32);
				REQURIED_MODULE_FIELD(ShellAPI);

				//Posix
				REQURIED_MODULE_FIELD(LibC);
				REQURIED_MODULE_FIELD(PThread);

				//Linux
				REQURIED_MODULE_FIELD(RT);
				
				//OSX
				REQURIED_MODULE_FIELD(System);
				REQURIED_MODULE_FIELD(DyLD);
				//! @endcond
			public:
				/*!
					@brief For loading optional AutoModules
					@par Thread Safety
						This function requires no thread safety
				*/
				Manager();
				/*!
					@brief For unloading optional AutoModules
					@par Thread Safety
						This function requires no thread safety
				*/
				~Manager();

				//! @copydoc CYB::Platform::Modules::AutoModule::Call()
				template <typename AIndexClass, typename... AArgs> auto Call(AArgs&&... AArguments);
				
				/*!
					@brief Check if an AutoModule is loaded
					@tparam AAutoModule The AutoModule to check
					@return true if the AutoModule is loaded, false otherwise
					@par Thread Safety
						This function requires no thread safety
					@attention This function will immediately return true for any type that is not an optional AutoModule
				*/
				template <template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> class AAutoModule> bool Loaded(void) noexcept;
				//! @copydoc CYB::Platform::Modules::AutoModule::Loaded()
				template <typename AIndexClass> bool Loaded(void) noexcept;
			};
		};
	};
};

#include "CYBModuleManager.inl"