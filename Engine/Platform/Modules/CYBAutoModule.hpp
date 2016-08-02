//! @file CYBAutoModule.hpp Defines the interface for creating templates that are modules as objects
#pragma once
namespace CYB {
	namespace Platform {
		namespace Modules {
			/*! 
				@brief AutoModule external helper for functions that rely on the state of AOptionalFunctions
				@tparam AOptionalFunctions Whether or not the AutoModule allows optional functions
				@tparam AN The number of functions the AutoModule will load
			*/
			template <bool AOptionalFunctions, unsigned int AN> class AutoModuleOptionalHelpers {};
			/*!
				@brief Automated intialization, function loading, and calling of module functions
				@tparam AN The number of functions the AutoModule will load<BR>
				@tparam AFunctionTypes The types of the functions being called
			*/
			template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> class AutoModule : private AutoModuleOptionalHelpers<AOptionalFunctions, AN> {
				ENABLE_TEST_HOOKS
			public:
				using FParameterPack = API::ParameterPack<AFunctionTypes...>;	//!< @brief The ParameterPack of the Module's functions
			private:
				Module FModule;	//!< @brief The owned module
				void* FFunctionPointers[AN];	//!< @brief Pointers to loaded functions
			private:
				/*!
					@brief Get the library name for this auto module. Must be implemented
					@return The names of the Module associated with this AutoModule
				*/
				static constexpr const char* ModuleName(void);
				/*!
					@brief Template for overriding the names of certain functions at compile time
					@return nullptr if there is no override, the name of a function otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				static const API::String::Static* OverridenNames(void) noexcept;
				/*!
					@brief The names of the functions associated with this AutoModule. Must be implemented;
					@return The names of the functions associated with this AutoModule
					@par Thread Safety
						This function requires no thread safety
				*/
				static const API::String::Static* FunctionNames(void) noexcept;
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
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_LOAD_FAILURE. Thrown if the module is unable to be loaded
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE. Thrown if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				AutoModule();
				AutoModule(AutoModule&& AMove);	//!< @brief See @ref structors
				AutoModule& operator=(AutoModule&& AMove);	//!< @brief See @ref structors
				~AutoModule();	//!< @brief See @ref structors

				/*!
					@brief Check if a function is loaded
					@tparam AIndexClass The index class of the function to check
					@return true if the function was loaded or AOptionalFunctions is false, false otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				template <class AIndexClass> bool Loaded(void) const noexcept;

				/*!
					@brief Call a loaded function
					@tparam AIndexClass The index class of the function to be called. Generated in CYB::Platform::Modules
					@tparam AArgs The types of the arguments of the function to call
					@param AArguments The arguments to the function represented by PointerIndex
					@return The result from the called function
					@par Thread Safety
						This function requires no thread safety. Does not apply to called function
					@attention Throws dependant on called function
				*/
				template<class AIndexClass, typename... AArgs> auto Call(AArgs&&... AArguments) const;
			};
		};
	};
};