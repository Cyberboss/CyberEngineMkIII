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