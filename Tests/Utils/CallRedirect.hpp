#pragma once

template <class AAutoModule, typename ACallable, unsigned int AFunctionIndex> class CallRedirectBase {
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

template <class AAutoModule, unsigned int AFunctionIndex> class CallRedirect : public CallRedirectBase<AAutoModule, typename AAutoModule::FParameterPack::template Indexer<AFunctionIndex>::FType, AFunctionIndex> {
public:
	typedef typename AAutoModule::FParameterPack::template Indexer<AFunctionIndex>::FType FCallable;
public:
	using CallRedirectBase<AAutoModule, FCallable, AFunctionIndex>::CallRedirectBase;
};


#include "CallRedirect.inl"