//! @file CYBAutoModule.inl Implements CYB::Engine::AutoModule
#pragma once

template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>::AutoModule() :
	FLibrary(ModuleName())
{
	auto Names(FunctionNames());
	for (unsigned int I(0); I < AN; ++I) {
		try {
			FFunctionPointers[I] = FLibrary.LoadFunction(Names[I]);
		}
		catch (Exception::SystemData AException) {
			if (OptionalFunctions() && AException.FErrorCode == Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE)
				FFunctionPointers[I] = nullptr;
			else
				throw AException;
		}
	}
}
template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>::AutoModule(AutoModule&& AMove) :
	FLibrary(std::move(AMove.FLibrary)),
	FFunctionPointers(AMove.FFunctionPointers)
{}
template <unsigned int AN, typename... AFunctionTypes> CYB::Platform::AutoModule<AN, AFunctionTypes...>& CYB::Platform::AutoModule<AN, AFunctionTypes...>::operator=(AutoModule&& AMove) {
	FLibrary = std::move(AMove.FLibrary);
	FFunctionPointers = AMove.FFunctionPointers;
	return *this;
}

template <unsigned int AN, typename... AFunctionTypes> template <unsigned int APointerIndex> bool CYB::Platform::AutoModule<AN, AFunctionTypes...>::Loaded(void) const {
	return FFunctionPointers[APointerIndex] != nullptr;
}

template <unsigned int AN, typename... AFunctionTypes> template <unsigned int APointerIndex, typename... Args> auto CYB::Platform::AutoModule<AN, AFunctionTypes...>::Call(Args&&... AArguments) const {
	typedef typename API::ParameterPackIndexer<APointerIndex, AFunctionTypes...>::type CallableType;
	static_assert(std::is_function<CallableType>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<CallableType*>(FFunctionPointers[APointerIndex]));
	return Callable(std::forward<Args>(AArguments)...);
}