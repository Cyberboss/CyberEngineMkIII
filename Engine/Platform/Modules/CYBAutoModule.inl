//! @file CYBAutoModule.inl Implements CYB::Engine::AutoModule
#pragma once

namespace CYB {
	namespace Platform {
		namespace Modules {
			/*!
				@brief AutoModule external helper construction function with missing function checks
				@tparam AN The number of functions the AutoModule will load
			*/
			template <unsigned int AN> class AutoModuleOptionalHelpers<true, AN> {
			protected:
				/*!
					@brief Shared constructor implementation for optional functions
					@param AModule The Module to construct from
					@param AFunctionPointers Function pointer list reference
					@param AFunctionNames Names of the functions to load from the Module
					@param AOverridenNames Names to override in @p AFunctionNames
					@par Thread Safety
						This function requires no thread safety
				*/
				static void Construct(Module& AModule, void* (&AFunctionPointers)[AN], const API::String::Static* const AFunctionNames, const API::String::Static* const AOverridenNames) noexcept {
					for (unsigned int I(0); I < AN; ++I)
						try {
							AFunctionPointers[I] = AModule.LoadFunction(AOverridenNames[I].RawLength() == 0 ? AFunctionNames[I] : AOverridenNames[I]);
						}
						catch (Exception::Internal AException) {
							API::Assert::Equal(AException.FErrorCode, static_cast<unsigned int>(Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE));
							AFunctionPointers[I] = nullptr;
						}
				}

				/*!
					@brief Check if a function is loaded
					@param AFunction A pointer to the function
					@return true if the function was loaded, false otherwise
					@par Thread Safety
						This function requires no thread safety
				*/
				static bool Loaded(const void* const AFunction) noexcept {
					return AFunction != nullptr;
				}
			};
			/*!
				@brief AutoModule external helper construction function without missing function checks
				@tparam AN The number of functions the AutoModule will load
			*/
			template <unsigned int AN> class AutoModuleOptionalHelpers<false, AN> {
			protected:
				/*!
					@brief Shared constructor implementation for required functions
					@param AModule The Module to construct from
					@param AFunctionPointers Function pointer list reference
					@param AFunctionNames Names of the functions to load from the Module
					@param AOverridenNames Names to override in @p AFunctionNames
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE. Thrown if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				static void Construct(Module& AModule, void* (&AFunctionPointers)[AN], const API::String::Static* const AFunctionNames, const API::String::Static* const AOverridenNames) {
					for (unsigned int I(0); I < AN; ++I)
						AFunctionPointers[I] = AModule.LoadFunction(AOverridenNames[I].RawLength() == 0 ? AFunctionNames[I] : AOverridenNames[I]);
				}

				/*!
					@brief Check if a function is loaded
					@param AFunction A pointer to the function
					@return true Since optional functions are not allowed in this type of AutoModule
				*/
				static constexpr bool Loaded(const void* const AFunction) noexcept {
					static_cast<void>(AFunction);
					return true;
				}
			};
		};
	};
};

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::AutoModule() :
	FModule(CYB::API::String::Static(ModuleName()))
{
	AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Construct(FModule, FFunctionPointers, FunctionNames(), OverridenNames());
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::~AutoModule() {
	for (unsigned int I(0); I < AN; ++I)
		FFunctionPointers[I] = nullptr;
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::AutoModule(AutoModule&& AMove) :
	FModule(std::move(AMove.FModule)),
	FFunctionPointers{ AMove.FFunctionPointers }
{}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>& CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::operator=(AutoModule&& AMove) {
	FModule = std::move(AMove.FModule);
	for(unsigned int I(0); I < AN; ++I)
		FFunctionPointers[I] = AMove.FFunctionPointers[I];
	return *this;
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> template <class AIndexClass> bool CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Loaded(void) const noexcept {
	return AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Loaded(FFunctionPointers[AIndexClass::Index]);
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> template <class AIndexClass, typename... AArgs> auto CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Call(AArgs&&... AArguments) const {
	static_assert(std::is_same<typename AIndexClass::FAutoModule, AutoModule<AOptionalFunctions, AN, AFunctionTypes...>>::value, "Unmatched AutoModule index class");
	using CallableType = typename FParameterPack::template Indexer<AIndexClass::Index>::FType;	//Typesafe af
	static_assert(std::is_function<CallableType>::value, "AutoModule call must refer to a function");
	API::Assert::True(Loaded<AIndexClass>());
	return (reinterpret_cast<CallableType*>(FFunctionPointers[AIndexClass::Index]))(std::forward<AArgs>(AArguments)...);	//https://www.youtube.com/watch?v=_X6VoFBCE9k
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> const CYB::API::String::Static* CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::OverridenNames(void) noexcept {
	static API::String::Static Empty[AN];
	for (auto I(0U); I < AN; ++I)
		Empty[I] = nullptr;
	return Empty;
}