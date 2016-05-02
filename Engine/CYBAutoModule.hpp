//! @file CYBAutoModule.hpp Defines the interface for creating templates that are modules as objects
#pragma once
namespace CYB {
	namespace Platform {
		/*! 
			@brief Automated intialization, function loading, and calling of module functions
			@par Params
				<B>N</B>: The number of functions the AutoModule will load<BR>
				<B>AFunctionTypes</B>: The types of the functions being called
		*/
		template <unsigned int N, typename... AFunctionTypes> class AutoModule {
		private:
			Module FLibrary;	//!< @brief The owned module
			void* FFunctionPointers[N];	//!< @brief Pointers to loaded functions
		private:
			/*!
				@brief Get the Library name for this auto module, must be implemented
				@return The names of the Module associated with this AutoModule
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			static constexpr const char* ModuleName(void);
			/*!
				@brief Get the functions for this auto module, must be implemented
				@return The names of the functions associated with this AutoModule
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					This function does not throw exceptions
			*/
			static constexpr const char* FunctionNames(void);
		public:
			/*!
				@brief Construct an AutoModule
				@par Thread Safety
					This function requires no thread safety
				@par Exception Safety
					CYB::Exception::SystemData::MODULE_LOAD_FAILURE if the module is unable to be loaded<BR>
					CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE if a requested function is unable to be loaded from the owned module
			*/
			AutoModule();
			AutoModule(AutoModule&& AMove);
			AutoModule& operator=(AutoModule&& AMove);

			template<unsigned int PointerIndex, typename... Args> auto Call(Args&&... AArguments);
		};
	};
};
