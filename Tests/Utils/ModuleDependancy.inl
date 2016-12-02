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

template <class AAutoModule> ModuleDependancy<AAutoModule>::ModuleDependancy() :
	FReferenceToManagerObject(GetAMReference<AAutoModule>())
{
	FReferenceToManagerObject = AAutoModule();
}
template <class AAutoModule> ModuleDependancy<AAutoModule>::~ModuleDependancy() {
	FReferenceToManagerObject.~AAutoModule();
}

template <class AAutoModule> template <class AIndexClass, template <class ARedirector> class ARedirectedClass> CallRedirect<AAutoModule, AIndexClass> ModuleDependancy<AAutoModule>::Redirect(void) {
	return CallRedirect<AAutoModule, AIndexClass>(FReferenceToManagerObject, reinterpret_cast<typename CallRedirect<AAutoModule, AIndexClass>::FCallable*>(ARedirectedClass<CallRedirect<AAutoModule, AIndexClass>>::RedirectedFunction));
}
