#pragma once

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> void* CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::FOldFunction(nullptr);

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::CallRedirectBase(AAutoModule& AReference, ACallable* const ANewFunction) :
	FReference(AReference),
	FMoved(false)
{
	CYB::API::Assert::Equal<decltype(FOldFunction)>(FOldFunction, nullptr);
	FOldFunction = ReassignAutoModuleFunctionPointer<AAutoModule>(FReference, AFunctionIndex, reinterpret_cast<void*>(ANewFunction));
}

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::CallRedirectBase(CallRedirectBase&& AMove) :
	FReference(AMove.FReference)
{
	AMove.FMoved = true;
}

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::~CallRedirectBase() {
	if (!FMoved) {
		ReassignAutoModuleFunctionPointer<AAutoModule>(FReference ,AFunctionIndex, FOldFunction);
		FOldFunction = nullptr;
		FMoved = true;
	}
}

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> template<typename... AArgs> auto CallRedirectBase<AAutoModule, ACallable, AFunctionIndex>::CallOriginal(AArgs&&... AArguments) {
	static_assert(std::is_function<ACallable>::value, "Call must refer to a function");
	auto Callable(reinterpret_cast<ACallable*>(FOldFunction));
	return Callable(std::forward<AArgs>(AArguments)...);
}

template <unsigned int AFunctionIndex> class CallRedirect<CYB::Platform::Modules::AMFake, AFunctionIndex> {
public:
	typedef void FCallable;
public:
	CallRedirect(CYB::Platform::Modules::AMFake& AReference, FCallable* const ANewFunction) {
		static_cast<void>(AReference);
		static_cast<void>(ANewFunction);
	}
	template <typename... AArgs> static auto CallOriginal(AArgs&&...) -> void { }
};
