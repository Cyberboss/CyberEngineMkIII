#pragma once
template <class AAutoModule> class ModuleDependancy {
private:
	AAutoModule& FReferenceToManagerObject;
public:
	ModuleDependancy();
	~ModuleDependancy();

	template <class AIndexClass, template <class ARedirector> class ARedirectedClass> CallRedirect<AAutoModule, AIndexClass> Redirect(void);
};

#include "ModuleDependancy.inl"