//! @file CYBModuleManager.inl Contains the definitions of CYB::Platform::JModules::Manager
#pragma once

inline CYB::Platform::Modules::Manager::Manager() {
	//Load optionals
}

inline CYB::Platform::Modules::Manager::~Manager() {
	//Unload optionals
}

template <typename ACallIndex, typename... AArgs> auto CYB::Platform::Modules::Manager::Call(AArgs&&... AArguments) {
	typename ACallIndex::FAutoModule* AM(GetAutoModule<typename ACallIndex::FAutoModule>());
	API::Assert::NotEqual<typename ACallIndex::FAutoModule*>(AM, nullptr);
	return AM->template Call<ACallIndex>(std::forward<AArgs>(AArguments)...);
}

template <template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> class AAutoModule> bool CYB::Platform::Modules::Manager::Loaded(void) noexcept {
	return GetAutoModule<AAutoModule>() != nullptr;
}

template <typename ACallIndex> bool CYB::Platform::Modules::Manager::Loaded(void) noexcept {
	return GetAutoModule<typename ACallIndex::FAutoModule>()->template Loaded<ACallIndex>();
}