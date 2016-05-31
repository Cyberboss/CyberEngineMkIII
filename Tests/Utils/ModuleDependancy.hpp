#pragma once
template <unsigned int APlatform, class AAutoModule> class ModuleDependancy {
private:
	AAutoModule& FReferenceToManagerObject;
public:
	ModuleDependancy(AAutoModule& AReferenceToManagerObject);
	~ModuleDependancy();

	template <unsigned int AIndex, template <class ARedirector> class ARedirectedClass> CallRedirect<AAutoModule, AIndex> Redirect(void);
};

#include "ModuleDependancy.inl"