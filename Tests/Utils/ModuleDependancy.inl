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

template <unsigned int APlatform, class AAutoModule> ModuleDependancy<APlatform, AAutoModule>::ModuleDependancy() :
	FReferenceToManagerObject(GetAMReference<AAutoModule>())
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
