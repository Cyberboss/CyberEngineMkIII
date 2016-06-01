//! @file CYBModule.cpp Shared function implementations for CYB::Platform::Module
#include "CYB.hpp"

CYB::Platform::Modules::Module::Module(const API::String::CStyle& AModuleName):
	Implementation::Module(AModuleName)
{}
CYB::Platform::Modules::Module::Module(Module&& AMove):
	Implementation::Module(std::move(AMove))
{}
CYB::Platform::Modules::Module& CYB::Platform::Modules::Module::operator=(Module&& AMove) {
	Implementation::Module::operator=(std::move(AMove));
	return *this;
}
//! @cond
CYB::Platform::Modules::Implementation::Module::Module(Module&& AMove) :
	FModule(AMove.FModule)
{
	AMove.FModule = nullptr;
}
CYB::Platform::Modules::Implementation::Module& CYB::Platform::Modules::Implementation::Module::operator=(Module&& AMove) {
	FModule = AMove.FModule;
	AMove.FModule = nullptr;
	return *this;
}
//! @endcond