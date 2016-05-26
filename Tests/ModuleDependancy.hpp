#pragma once
template <unsigned int APlatform, class AAutoModule> class ModuleDependancy {
private:
	AAutoModule& FReferenceToManagerObject;
public:
	ModuleDependancy(AAutoModule& AReferenceToManagerObject);
	~ModuleDependancy();
};

#include "ModuleDependancy.inl"