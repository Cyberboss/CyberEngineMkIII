#pragma once

template <class AAutoModule, typename ACallable, class AIndexClass> void* CallRedirectBase<AAutoModule, ACallable, AIndexClass>::FOldFunction(nullptr);

template <class AAutoModule, typename ACallable, class AIndexClass> CallRedirectBase<AAutoModule, ACallable, AIndexClass>::CallRedirectBase(AAutoModule& AReference, ACallable* const ANewFunction) :
	FReference(AReference),
	FMoved(false)
{
	CYB::API::Assert::Equal<decltype(FOldFunction)>(FOldFunction, nullptr);
	FOldFunction = ReassignAutoModuleFunctionPointer<AAutoModule>(FReference, AIndexClass::Index, reinterpret_cast<void*>(ANewFunction));
}

template <class AAutoModule, typename ACallable, class AIndexClass> CallRedirectBase<AAutoModule, ACallable, AIndexClass>::CallRedirectBase(CallRedirectBase&& AMove) :
	FReference(AMove.FReference)
{
	AMove.FMoved = true;
}

template <class AAutoModule, typename ACallable, class AIndexClass> CallRedirectBase<AAutoModule, ACallable, AIndexClass>::~CallRedirectBase() {
	if (!FMoved) {
		ReassignAutoModuleFunctionPointer<AAutoModule>(FReference , AIndexClass::Index, FOldFunction);
		FOldFunction = nullptr;
		FMoved = true;
	}
}

template <class AAutoModule, typename ACallable, class AIndexClass> template<typename... AArgs> auto CallRedirectBase<AAutoModule, ACallable, AIndexClass>::CallOriginal(AArgs&&... AArguments) {
	static_assert(std::is_function<ACallable>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<ACallable*>(FOldFunction));
	return Callable(std::forward<AArgs>(AArguments)...);
}

template <class AIndexClass> class CallRedirect<CYB::Platform::Modules::AMFake, AIndexClass> {
public:
	typedef void FCallable;
public:
	CallRedirect(CYB::Platform::Modules::AMFake& AReference, FCallable* const ANewFunction) {
		static_cast<void>(AReference);
		static_cast<void>(ANewFunction);
	}
	template <typename... AArgs> static auto CallOriginal(AArgs&&...) -> void { }
};
