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
				
				/*!
					@brief Check if an AutoModule is loaded
					@tparam AAutoModule The AutoModule to check
					@return true if the AutoModule is loaded, false otherwise
					@par Thread Safety
						This function requires no thread safety
					@attention This function will immediately return true for any type that is not an optional AutoModule
				*/
				template <typename AAutoModule> bool LoadedInternal(void) const noexcept;
			private:
				//! @cond
				//Windows
				REQUIRED_MODULE_FIELD(Kernel32);
				REQUIRED_MODULE_FIELD(Kernel32Extended);
				REQUIRED_MODULE_FIELD(Shell);
				REQUIRED_MODULE_FIELD(Ole32);
				REQUIRED_MODULE_FIELD(ShellAPI);

				//Posix
				REQUIRED_MODULE_FIELD(LibC);
				REQUIRED_MODULE_FIELD(PThread);

				//Linux
				REQUIRED_MODULE_FIELD(RT);
				
				//OSX
				REQUIRED_MODULE_FIELD(System);
				REQUIRED_MODULE_FIELD(DyLD);
				//! @endcond
			public:
				/*!
					@brief For loading optional AutoModules
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_LOAD_FAILURE. Thrown if a module is unable to be loaded
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE. Thrown if a requested function is unable to be loaded from a required module
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
				template <template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> class AAutoModule> bool Loaded(void) const noexcept;
				//! @copydoc CYB::Platform::Modules::AutoModule::Loaded()
				template <typename AIndexClass> bool Loaded(void) noexcept;
			};
		};
	};
};

#include "CYBModuleManager.inl"