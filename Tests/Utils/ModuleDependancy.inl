#pragma once

template <unsigned int APlatform, class AAutoModule> ModuleDependancy<APlatform, AAutoModule>::ModuleDependancy(AAutoModule& AReferenceToManagerObject) :
	FReferenceToManagerObject(AReferenceToManagerObject)
{
	if(APlatform & CYB::API::Platform::Current())
		FReferenceToManagerObject = AAutoModule();
}
template <unsigned int APlatform, class AAutoModule> ModuleDependancy<APlatform, AAutoModule>::~ModuleDependancy() {
	if (APlatform & CYB::API::Platform::Current())
		FReferenceToManagerObject.~AAutoModule();
}

template <unsigned int APlatform, class AAutoModule> template <class AIndexClass, template <class ARedirector> class ARedirectedClass> CallRedirect<AAutoModule, AIndexClass> ModuleDependancy<APlatform, AAutoModule>::Redirect(void) {
	return CallRedirect<AAutoModule, AIndexClass>(FReferenceToManagerObject, reinterpret_cast<typename CallRedirect<AAutoModule, AIndexClass>::FCallable*>(ARedirectedClass<CallRedirect<AAutoModule, AIndexClass>>::RedirectedFunction));
}
