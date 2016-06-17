//! @file CYBAutoModule.inl Implements CYB::Engine::AutoModule
#pragma once

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::AutoModule() :
	FModule(CYB::API::String::Static(ModuleName()))
{
	void* NoReplacedFunctions[AN];
	for (unsigned int I(0); I < AN; ++I)
		NoReplacedFunctions[I] = nullptr;
	AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Construct(FModule, FFunctionPointers, NoReplacedFunctions, FunctionNames());
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::AutoModule(void* const (&AReplacedFunctions)[AN]) :
	FModule(CYB::API::String::Static(ModuleName()))
{
	AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Construct(FModule, FFunctionPointers, AReplacedFunctions, FunctionNames());
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::~AutoModule() {
	for (unsigned int I(0); I < AN; ++I)
		FFunctionPointers[I] = nullptr;
}
template <unsigned int AN> void CYB::Platform::Modules::AutoModuleOptionalHelpers<true, AN>::Construct(Module& AModule, void* (&AFunctionPointers)[AN], void* const (&AReplacedFunctions)[AN], const API::String::Static* const AFunctionNames) {
	for (unsigned int I(0); I < AN; ++I)
		try {
			AFunctionPointers[I] = AReplacedFunctions[I] != nullptr ? AReplacedFunctions[I] : AModule.LoadFunction(AFunctionNames[I]);
		}
		catch (Exception::SystemData AException) {
			API::Assert::Equal(AException.FErrorCode, static_cast<unsigned int>(Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE));
			AFunctionPointers[I] = nullptr;
		}
}

template <unsigned int AN> void CYB::Platform::Modules::AutoModuleOptionalHelpers<false, AN>::Construct(Module& AModule, void* (&AFunctionPointers)[AN], void* const (&AReplacedFunctions)[AN], const API::String::Static* const AFunctionNames) {
	for (unsigned int I(0); I < AN; ++I)
		AFunctionPointers[I] = AReplacedFunctions[I] != nullptr ? AReplacedFunctions[I] : AModule.LoadFunction(AFunctionNames[I]);
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

template <unsigned int AN> bool CYB::Platform::Modules::AutoModuleOptionalHelpers<true, AN>::Loaded(const void* const AFunction) noexcept {
	return AFunction != nullptr;
}

template <unsigned int AN> constexpr bool CYB::Platform::Modules::AutoModuleOptionalHelpers<false, AN>::Loaded(const void* const AFunction) {
	static_cast<void>(AFunction);
	return true;
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> bool CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Loaded(const unsigned int AFunctionIndex) const noexcept {
	return AutoModuleOptionalHelpers<AOptionalFunctions, AN>::Loaded(FFunctionPointers[AFunctionIndex]);
}

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> template <unsigned int APointerIndex, typename... AArgs> auto CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Call(AArgs&&... AArguments) const {
	typedef typename API::ParameterPack<AFunctionTypes...> AsParameterPack;
	typedef typename AsParameterPack::template Indexer<APointerIndex> Indexer;
	typedef typename Indexer::FType CallableType;
	static_assert(std::is_function<CallableType>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<CallableType*>(FFunctionPointers[APointerIndex]));
	return Callable(std::forward<AArgs>(AArguments)...);
}