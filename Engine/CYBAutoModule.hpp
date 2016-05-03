//! @file CYBAutoModule.hpp Defines the interface for creating templates that are modules as objects
#pragma once
namespace CYB {
	namespace Platform {
		/*! 
			@brief Automated intialization, function loading, and calling of module functions
			@tparam AN The number of functions the AutoModule will load<BR>
			@tparam AFunctionTypes The types of the functions being called
		*/
		template <unsigned int AN, typename... AFunctionTypes> class AutoModule {
		private:
			Module FLibrary;	//!< @brief The owned module
			void* FFunctionPointers[AN];	//!< @brief Pointers to loaded functions
		private:
			/*!
				@brief Get the library name for this auto module. Must be implemented
				@return The names of the Module associated with this AutoModule
			*/
			static constexpr const char* ModuleName(void);
			/*!
				@brief The names of the functions associated with this AutoModule. Must be implemented;
				@return The names of the functions associated with this AutoModule
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			static const char* const* FunctionNames(void);
		public:
			/*!
				@brief Checks if functions loaded for this auto module will be optional. If true functions should be checked with Loaded to make sure they exist. Must be implemented
				@return true if the AutoModule loads functions optionally, false otherwise
			*/
			static constexpr bool OptionalFunctions(void);
			/*!
				@brief Construct an AutoModule
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MODULE_LOAD_FAILURE if the module is unable to be loaded<BR>
					CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
			*/
			AutoModule();
			AutoModule(AutoModule&& AMove);
			AutoModule& operator=(AutoModule&& AMove);

			/*!
				@brief Check if a function is loaded
				@tparam APointerIndex The enum value of the function to be checked. Generated in CYB::Platform::Implementation::Modules
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			template<unsigned int APointerIndex> bool Loaded(void) const;
			/*!
				@brief Call a loaded function
				@tparam APointerIndex The enum value of the function to be called. Generated in CYB::Platform::Implementation::Modules
				@tparam AArgs The types of the arguments of the function to call
				@param AArguments The arguments to the function represented by PointerIndex
				@return The result from the called function
				@par Thread Safety
					This function requires no thread safety. Does not apply to called function
				@par Exception Safety
					This function will throw any exception from the called function
			*/
			template<unsigned int APointerIndex, typename... AArgs> auto Call(AArgs&&... AArguments) const;
		};
	};
};
