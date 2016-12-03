//! @file CYBModuleManager.inl Contains the definitions of CYB::Platform::JModules::Manager
#pragma once

//! @cond
//Windows
REQUIRED_MODULE_MANAGEMENT(Kernel32)
REQUIRED_MODULE_MANAGEMENT(Kernel32Extended)
REQUIRED_MODULE_MANAGEMENT(Shell)
REQUIRED_MODULE_MANAGEMENT(Ole32)
REQUIRED_MODULE_MANAGEMENT(ShellAPI)

//Posix
REQUIRED_MODULE_MANAGEMENT(LibC)
REQUIRED_MODULE_MANAGEMENT(PThread)

//Linux
REQUIRED_MODULE_MANAGEMENT(RT)

//OSX
REQUIRED_MODULE_MANAGEMENT(System)
REQUIRED_MODULE_MANAGEMENT(DyLD)
//! @endcond

inline CYB::Platform::Modules::Manager::Manager() {
	//Load optionals
}

inline CYB::Platform::Modules::Manager::~Manager() {
	//Unload optionals
}

template <typename AIndexClass, typename... AArgs> auto CYB::Platform::Modules::Manager::Call(AArgs&&... AArguments) {
	typename AIndexClass::FAutoModule* const AM(GetAutoModule<typename AIndexClass::FAutoModule>());
	API::Assert::NotEqual<decltype(AM)>(AM, nullptr);
	return AM->template Call<AIndexClass>(std::forward<AArgs>(AArguments)...);
}

template <template <bool AOptionalFunctions, unsigned int AN, typename... AFunctionTypes> class AAutoModule> bool CYB::Platform::Modules::Manager::Loaded(void) const noexcept {
	return LoadedInternal<AAutoModule>();
}

template <typename AIndexClass> bool CYB::Platform::Modules::Manager::Loaded(void) noexcept {
	return GetAutoModule<typename AIndexClass::FAutoModule>()->template Loaded<AIndexClass>();
}