//! @file CYBModule.cpp Shared function implementations for CYB::Platform::Module
#include "CYB.hpp"

CYB::Platform::Modules::Module::Module(const API::String::CStyle& AModuleName):
	Implementation::Module(AModuleName)
{}

//! @cond
CYB::Platform::Modules::Implementation::Module::Module(Module&& AMove) noexcept :
	FModule(AMove.FModule)
{
	AMove.FModule = nullptr;
}
CYB::Platform::Modules::Implementation::Module& CYB::Platform::Modules::Implementation::Module::operator=(Module&& AMove) noexcept {
	FModule = AMove.FModule;
	AMove.FModule = nullptr;
	return *this;
}
//! @endcond