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
					@brief Shared constructor implementation
					@param AModule The Module to construct from
					@param AFunctionPointers Function pointer list reference
					@param AReplacedFunctions Function pointers to be used in place of the regular module functions. If any are nullptr, they will instead be loaded from the module as normal
					@param AFunctionNames Names of the functions to load from the Module
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE. Thrown if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				static void Construct(Module& AModule, void* (&AFunctionPointers)[AN], void* const (&AReplacedFunctions)[AN], const API::String::Static* const* const AFunctionNames) {
					for (unsigned int I(0); I < AN; ++I)
						try {
						AFunctionPointers[I] = AReplacedFunctions[I] != nullptr ? AReplacedFunctions[I] : AModule.LoadFunction(*AFunctionNames[I]);
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
					@brief Shared constructor implementation
					@param AModule The Module to construct from
					@param AFunctionPointers Function pointer list reference
					@param AReplacedFunctions Function pointers to be used in place of the regular module functions. If any are nullptr, they will instead be loaded from the module as normal
					@param AFunctionNames Names of the functions to load from the Module
					@par Thread Safety
						This function requires no thread safety
					@throws CYB::Exception::Internal Error code: CYB::Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE. Thrown if a requested function is unable to be loaded from the owned module, unless OptionalFunctions returns true
				*/
				static void Construct(Module& AModule, void* (&AFunctionPointers)[AN], void* const (&AReplacedFunctions)[AN], const API::String::Static* const* const AFunctionNames) {
					for (unsigned int I(0); I < AN; ++I)
						AFunctionPointers[I] = AReplacedFunctions[I] != nullptr ? AReplacedFunctions[I] : AModule.LoadFunction(*AFunctionNames[I]);
				}

				/*!
					@brief Check if a function is loaded
					@param AFunction A pointer to the function
					@return true Since optional functions are not allowed in this type of AutoModule
				*/
				static constexpr bool Loaded(const void* const AFunction) {
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
	void* NoReplacedFunctions[AN];
	for (unsigned int I(0); I < AN; ++I)
		NoReplacedFunctions[I] = nullptr;
	const API::String::Static* FunctionNameList[AN];
	for (auto I(0U); I < AN; ++I)
		FunctionNameList[I] = OverridenNames()[I].Length() == 0 ? &FunctionNames()[I] : &OverridenNames()[I];
	AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Construct(FModule, FFunctionPointers, NoReplacedFunctions, FunctionNameList);
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::AutoModule(void* const (&AReplacedFunctions)[AN]) :
	FModule(CYB::API::String::Static(ModuleName()))
{
	const API::String::Static* FunctionNameList[AN];
	for (auto I(0U); I < AN; ++I)
		FunctionNameList[I] = OverridenNames()[I].Length() == 0 ? &FunctionNames()[I] : &OverridenNames()[I];
	AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Construct(FModule, FFunctionPointers, AReplacedFunctions, FunctionNameList);
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

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> bool CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Loaded(const unsigned int AFunctionIndex) const noexcept {
	return AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Loaded(FFunctionPointers[AFunctionIndex]);
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> template <unsigned int APointerIndex, typename... AArgs> auto CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Call(AArgs&&... AArguments) const {
	typedef typename API::ParameterPack<AFunctionTypes...> AsParameterPack;
	typedef typename AsParameterPack::template Indexer<APointerIndex> Indexer;
	typedef typename Indexer::FType CallableType;
	static_assert(std::is_function<CallableType>::value, "Call must refer to a function");
	API::Assert::True(Loaded(APointerIndex));
	auto Callable(reinterpret_cast<CallableType*>(FFunctionPointers[APointerIndex]));
	return Callable(std::forward<AArgs>(AArguments)...);
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> const CYB::API::String::Static* CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::OverridenNames(void) noexcept {
	static API::String::Static Empty[AN];
	for (auto I(0U); I < AN; ++I)
		Empty[I] = nullptr;
	return Empty;
}