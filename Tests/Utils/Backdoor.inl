#pragma once

template <class AAutoModule> struct AutoModuleHook {
	AAutoModule& FAutoModule;
	void*& FPointerSwap;
	const unsigned int FIndex;
};

template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> template <class AAutoModuleHook> void CYB::Platform::Modules::AutoModule<AOptionalFunctions, AN, AFunctionTypes...>::Backdoor(AAutoModuleHook& AHooker) {
	auto Old(AHooker.FAutoModule.FFunctionPointers[AHooker.FIndex]);
	AHooker.FAutoModule.FFunctionPointers[AHooker.FIndex] = AHooker.FPointerSwap;
	AHooker.FPointerSwap = Old;
}

template<class AAutoModule> void* ReassignAutoModuleFunctionPointer(AAutoModule& AModule, const unsigned int AIndex, void* ANewPointer) {
	AutoModuleHook<AAutoModule> Hook{ AModule, ANewPointer, AIndex };
	AAutoModule::template Backdoor<AutoModuleHook<AAutoModule>>(Hook);
	return ANewPointer;
}