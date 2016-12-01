#pragma once
template <unsigned int APlatform, class AAutoModule> class ModuleDependancy {
private:
	AAutoModule& FReferenceToManagerObject;
public:
	ModuleDependancy();
	~ModuleDependancy();

	template <class AIndexClass, template <class ARedirector> class ARedirectedClass> CallRedirect<AAutoModule, AIndexClass> Redirect(void);
};

#include "ModuleDependancy.inl"