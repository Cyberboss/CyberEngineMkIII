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
				@brief AutoModule external helper construction function with missing function checks
				@tparam AN The number of functions the AutoModule will load
			*/
			template <unsigned int AN> class AutoModuleOptionalHelpers<true, AN> {
			protected:
				/*!
					@brief Shared constructor implementation
					@param AModule The Module to construct from
					@param AFunctionPointers Function pointer list reference
					@param AReplacedFunctions Function pointers to be used in place of the regular module functions. If any are nullptr, they will instead be loaded from the module as normal
					@param AFunctionNames Names of the functions to load from the Module
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				static void Construct(Module& AModule, void* (&AFunctionPointers)[AN], void* const (&AReplacedFunctions)[AN], const API::String::Static* const AFunctionNames);

				/*!
					@brief Check if a function is loaded
					@param AFunction A pointer to the function
					@return true if the function was loaded, false otherwise
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static bool Loaded(const void* const AFunction) noexcept;
			};
			/*!
				@brief AutoModule external helper construction function without missing function checks
				@tparam AN The number of functions the AutoModule will load
			*/
			template <unsigned int AN> class AutoModuleOptionalHelpers<false, AN> {
			protected:
				/*!
					@brief Shared constructor implementation
					@param AModule The Module to construct from
					@param AFunctionPointers Function pointer list reference
					@param AReplacedFunctions Function pointers to be used in place of the regular module functions. If any are nullptr, they will instead be loaded from the module as normal
					@param AFunctionNames Names of the functions to load from the Module
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				static void Construct(Module& AModule, void* (&AFunctionPointers)[AN], void* const (&AReplacedFunctions)[AN], const API::String::Static* const AFunctionNames);

				/*!
					@brief Check if a function is loaded
					@param AFunction A pointer to the function
					@return true Since optional functions are not allowed in this type of AutoModule
				*/
				static constexpr bool Loaded(const void* const AFunction);
			};
			/*!
				@brief Automated intialization, function loading, and calling of module functions
				@tparam AN The number of functions the AutoModule will load<BR>
				@tparam AFunctionTypes The types of the functions being called
			*/
			template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> class AutoModule : private AutoModuleOptionalHelpers<AOptionalFunctions, AN> {
			public:
				typedef API::ParameterPack<AFunctionTypes...> FParameterPack;
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
					@brief The names of the functions associated with this AutoModule. Must be implemented;
					@return The names of the functions associated with this AutoModule
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				static const API::String::Static* FunctionNames(void);
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
				/*!
					@brief Construct an AutoModule with specific function pointers. Still loads the owned module
					@param AReplacedFunctions Function pointers to be used in place of the regular module functions. If any are nullptr, they will instead be loaded from the module as normal
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						CYB::Exception::SystemData::MODULE_LOAD_FAILURE if the module is unable to be loaded<BR>
						CYB::Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				AutoModule(void* const (&AReplacedFunctions)[AN]);
				AutoModule(AutoModule&& AMove);	//!< @brief See @ref structors
				AutoModule& operator=(AutoModule&& AMove);	//!< @brief See @ref structors
				~AutoModule();	//!< @brief See @ref structors

				/*!
					@brief Check if a function is loaded
					@param AFunctionIndex The index of the function to check
					@return true if the function was loaded or AOptionalFunctions is false, false otherwise
					@par Thread Safety
						This function requires no thread safety
					@par Exception Safety
						This function does not throw exceptions
				*/
				bool Loaded(const unsigned int AFunctionIndex) const noexcept;

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
#ifdef CYB_BUILDING_TESTS
				void* ReassignFunctionPointer(const unsigned int AIndex, void* ANewPointer);
#endif
			};
		};
	};
};