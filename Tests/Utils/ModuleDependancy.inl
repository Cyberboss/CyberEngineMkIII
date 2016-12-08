#pragma once

template <typename AAutoModule> struct RefHookStruc {
	void* FAM;
};

template <typename AAutoModule> void CYB::Platform::Modules::Manager::Backdoor(AAutoModule& AHooker) {
	reinterpret_cast<RefHookStruc<AAutoModule>&>(AHooker).FAM = CYB::Core().FModuleManager.GetAutoModule<AAutoModule>();
}

template <typename AAutoModule> AAutoModule& GetAMReference(void) {
	RefHookStruc<AAutoModule> Hooker;
	CYB::Platform::Modules::Manager::Backdoor<AAutoModule>(reinterpret_cast<AAutoModule&>(Hooker));
	return *static_cast<AAutoModule*>(Hooker.FAM);
}

template <class AAutoModuleClass> ModuleDependancy<AAutoModuleClass>::ModuleDependancy() :
	FReferenceToManagerObject(GetAMReference<typename AAutoModuleClass::FAutoModule>())
{
	using AAutoModule = typename AAutoModuleClass::FAutoModule;
	FReferenceToManagerObject = AAutoModule();
}
template <class AAutoModuleClass> ModuleDependancy<AAutoModuleClass>::~ModuleDependancy() {
	using AAutoModule = typename AAutoModuleClass::FAutoModule;
	FReferenceToManagerObject.~AAutoModule();
}

template <class AAutoModuleClass> template <class AIndexClass, template <class ARedirector> class ARedirectedClass> CallRedirect<typename AAutoModuleClass::FAutoModule, AIndexClass> ModuleDependancy<AAutoModuleClass>::Redirect(void) {
	using CRType = CallRedirect<typename AAutoModuleClass::FAutoModule, AIndexClass>;
	return CRType(FReferenceToManagerObject, reinterpret_cast<typename CRType::FCallable*>(ARedirectedClass<CRType>::RedirectedFunction));
}
