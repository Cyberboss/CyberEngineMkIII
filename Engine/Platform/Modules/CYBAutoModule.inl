//! @file CYBAutoModule.inl Implements CYB::Engine::AutoModule
#pragma once

template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>::AutoModule() :
	FLibrary(API::String::Static(ModuleName()))
{
	void* NoReplacedFunctions[AN];
	for (unsigned int I(0); I < AN; ++I)
		NoReplacedFunctions[I] = nullptr;
	Construct(NoReplacedFunctions);
}
template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>::AutoModule(void* const (&AReplacedFunctions)[AN]) :
	FLibrary(API::String::Static(ModuleName()))
{
	Construct(AReplacedFunctions);
}
template <unsigned int AN, typename... AFunctionTypes> void CYB::Platform::AutoModule<AN, AFunctionTypes...>::Construct(void* const (&AReplacedFunctions)[AN]) {
	auto Names(FunctionNames());
	for (unsigned int I(0); I < AN; ++I) {
		try {
			FFunctionPointers[I] = AReplacedFunctions[I] != nullptr ? AReplacedFunctions[I] : FLibrary.LoadFunction(Names[I]);
		}
		catch (Exception::SystemData AException) {
			if (OptionalFunctions() && AException.FErrorCode == Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE)
				FFunctionPointers[I] = nullptr;
			else
				throw;
		}
	}
}
template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>::AutoModule(AutoModule&& AMove) :
	FLibrary(std::move(AMove.FLibrary)),
	FFunctionPointers(AMove.FFunctionPointers)
{}
template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>& CYB::Platform::AutoModule<AN, AFunctionTypes...>::operator=(AutoModule&& AMove) {
	FLibrary = std::move(AMove.FLibrary);
	for(unsigned int I(0); I < AN; ++I)
		FFunctionPointers[I] = AMove.FFunctionPointers[I];
	return *this;
}

template <unsigned int AN, typename... AFunctionTypes> template <unsigned int APointerIndex> bool CYB::Platform::AutoModule<AN, AFunctionTypes...>::Loaded(void) const {
	return FFunctionPointers[APointerIndex] != nullptr;
}

template <unsigned int AN, typename... AFunctionTypes> template <unsigned int APointerIndex, typename... AArgs> auto CYB::Platform::AutoModule<AN, AFunctionTypes...>::Call(AArgs&&... AArguments) const {
	typedef typename API::ParameterPack<AFunctionTypes...> AsParameterPack;
	typedef typename AsParameterPack::template Indexer<APointerIndex> Indexer;
	typedef typename Indexer::FType CallableType;
	static_assert(std::is_function<CallableType>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<CallableType*>(FFunctionPointers[APointerIndex]));
	return Callable(std::forward<AArgs>(AArguments)...);
}

#ifdef CYB_BUILDING_TESTS
template <unsigned int AN, typename... AFunctionTypes> void* CYB::Platform::AutoModule<AN, AFunctionTypes...>::ReassignFunctionPointer(const unsigned int AIndex, void* const ANewPointer) {
	auto Old(FFunctionPointers[AIndex]);
	FFunctionPointers[AIndex] = ANewPointer;
	return Old;
}
#endif