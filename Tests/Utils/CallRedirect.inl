#pragma once

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> void* CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::FOldFunction(nullptr);

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::CallRedirectBase(AAutoModule& AReference, ACallable* const ANewFunction) :
	FReference(AReference),
	FMoved(false)
{
	CYB::API::Assert(FOldFunction == nullptr);
	FOldFunction = FReference.ReassignFunctionPointer(AFunctionIndex, reinterpret_cast<void*>(ANewFunction));
}

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::CallRedirectBase(CallRedirectBase&& AMove) :
	FReference(AMove.FReference)
{
	AMove.FMoved = true;
}

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::~CallRedirectBase() {
	if (!FMoved) {
		FReference.ReassignFunctionPointer(AFunctionIndex, FOldFunction);
		FOldFunction = nullptr;
	}
}

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> template<typename... AArgs> auto CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::CallOriginal(AArgs&&... AArguments) {
	static_assert(std::is_function<ACallable>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<ACallable*>(FOldFunction));
	return Callable(std::forward<AArgs>(AArguments)...);
}

template <unsigned int AFunctionIndex> class CallRedirect<CYB::Platform::ModuleDefinitions::AMFake, AFunctionIndex> {
public:
	typedef void FCallable;
public:
	CallRedirect(CYB::Platform::ModuleDefinitions::AMFake& AReference, FCallable* const ANewFunction) {
		static_cast<void>(AReference);
		static_cast<void>(ANewFunction);
	}
	template <typename... AArgs> static auto CallOriginal(AArgs&&...) -> void { }
};
