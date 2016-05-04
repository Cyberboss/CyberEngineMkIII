//! @file CYBModule.cpp Shared function implementations for CYB::Platform::Module
#include "CYB.hpp"

CYB::Platform::Module::Module(const API::String::CStyle& AModuleName):
	Implementation::Module(AModuleName)
{}
CYB::Platform::Module::Module(Module&& AMove):
	Implementation::Module(std::move(AMove))
{}
CYB::Platform::Implementation::Module::Module(Module&& AMove) :
	FModule(AMove.FModule)
{
	AMove.FModule = nullptr;
}
CYB::Platform::Implementation::Module& CYB::Platform::Implementation::Module::operator=(Module&& AMove) {
	FModule = AMove.FModule;
	AMove.FModule = nullptr;
	return *this;
}
CYB::Platform::Module& CYB::Platform::Module::operator=(Module&& AMove) {
	Implementation::Module::operator=(std::move(AMove));
	return *this;
}