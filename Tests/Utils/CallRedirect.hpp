#pragma once

template <class AAutoModule, typename ACallable, class AIndexClass> class CallRedirectBase {
public:
	static void* FOldFunction;
private:
	AAutoModule& FReference;
	bool FMoved;
public:
	CallRedirectBase(AAutoModule& AReference, ACallable* const ANewFunction);
	CallRedirectBase(CallRedirectBase&& AMove);
	~CallRedirectBase();

	template <typename... AArgs> static auto CallOriginal(AArgs&&... AArguments);
};

template <class AAutoModule, class AIndexClass> class CallRedirect : public CallRedirectBase<AAutoModule, typename AAutoModule::FParameterPack::template Indexer<AIndexClass::Index>::FType, AIndexClass> {
public:
	typedef typename AAutoModule::FParameterPack::template Indexer<AIndexClass::Index>::FType FCallable;
public:
	using CallRedirectBase<AAutoModule, FCallable, AIndexClass>::CallRedirectBase;
};


#include "CallRedirect.inl"