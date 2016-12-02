#pragma once
template <class AAutoModuleClass> class ModuleDependancy {
private:
	typename AAutoModuleClass::FAutoModule& FReferenceToManagerObject;
public:
	ModuleDependancy();
	~ModuleDependancy();

	template <class AIndexClass, template <class ARedirector> class ARedirectedClass> CallRedirect<typename AAutoModuleClass::FAutoModule, AIndexClass> Redirect(void);
};

#include "ModuleDependancy.inl"