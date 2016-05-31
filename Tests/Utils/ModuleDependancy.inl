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

template <unsigned int APlatform, class AAutoModule> template <unsigned int AIndex, template <class ARedirector> class ARedirectedClass> CallRedirect<AAutoModule, AIndex> ModuleDependancy<APlatform, AAutoModule>::Redirect(void) {
	return CallRedirect<AAutoModule, AIndex>(FReferenceToManagerObject, reinterpret_cast<typename CallRedirect<AAutoModule, AIndex>::FCallable*>(ARedirectedClass<CallRedirect<AAutoModule, AIndex>>::Redirect));
}
