//! @file CYBModule.hpp Defines the interface for loading module functions
#pragma once
namespace CYB {
	namespace Platform {
		//! @brief Encompasses a handle on loaded modules
		class Module : protected Implementation::Module {
		public:
			/*!
				@brief Constructs a module reference. The module named by @p ANonSuffixedModuleName will be loaded into the process' memory space if it is not already there
				@param AModuleName The name of the module to load excluding the platform specific extension or the 'lib' prefix
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MODULE_LOAD_FAILURE if the module is unable to be loaded
			*/
			Module(const API::String::CStyle& AModuleName);
			Module(const Module&) = delete;
			Module(Module&& AMove);
			Module& operator=(Module&& AMove);
			/*!
				@brief Destroys a module reference. If all references to a module are destroyed, functions loaded from those modules become invalid
			*/
			~Module() = default;

			/*!
				@brief Load a pointer to a function from a module. Pointer valid for as long as one reference to the owned module exists
				@param AFunctionName The ANSI name of the function to load
				@return A pointer which can be casted to the function's prototype and called
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE if the requested function is unable to be loaded from the owned module
			*/
			void* LoadFunction(const API::String::CStyle& AFunctionName);
		};
	};
};