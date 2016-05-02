//! @file CYBAutoModule.inl Implements CYB::Engine::AutoModule
#pragma once

template <unsigned int N, typename... AFunctionTypes> CYB::Platform::AutoModule<N, AFunctionTypes...>::AutoModule() :
	FLibrary(ModuleName())
{
	auto Names(FunctionNames());
	for (unsigned int I(0); I < N; ++I)
		FFunctionPointers = FLibrary.LoadFunction(Names[I]);
}
template <unsigned int N, typename... AFunctionTypes> CYB::Platform::AutoModule<N, AFunctionTypes...>::AutoModule(AutoModule&& AMove) :
	FLibrary(std::move(AMove.FLibrary))
{
	auto Names(FunctionNames());
	for (unsigned int I(0); I < N; ++I)
		FFunctionPointers = FLibrary.LoadFunction(Names[I]);
}
template <unsigned int N, typename... AFunctionTypes> CYB::Platform::AutoModule<N, AFunctionTypes...>& CYB::Platform::AutoModule<N, AFunctionTypes...>::operator=(AutoModule&& AMove) {
	FLibrary = std::move(AMove.FLibrary);
	FFunctionPointers = AMove.FFunctionPointers;
	return *this;
}
template <unsigned int N, typename... AFunctionTypes> template <unsigned int PointerIndex, typename... Args> auto CYB::Platform::AutoModule<N, AFunctionTypes...>::Call(Args&&... AArguments) {
	typedef typename API::ParameterPackIndexer::GetType<PointerIndex, AFunctionTypes...>::type CallableType;
	static_assert(std::is_function<CallableType>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<CallableType*>(FFunctionPointers[PointerIndex]));
	return Callable(std::forward<Args>(AArguments)...);
}