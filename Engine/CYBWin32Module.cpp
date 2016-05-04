//! @file CYBWin32Module.cpp Implements CYB::Platform::Module for Win32

#include "CYB.hpp"

CYB::Platform::Implementation::Module::Module(const CYB::API::String::CStyle& AModuleName) :
	FModule(Win32::LoadLibraryA(AModuleName.CString()))
{
	if (FModule == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_LOAD_FAILURE);
}
CYB::Platform::Implementation::Module::~Module() {
	if (FModule != nullptr)
		Win32::FreeLibrary(FModule);
}

void* CYB::Platform::Module::LoadFunction(const CYB::API::String::CStyle& AFunctionName) {
	auto Result(Implementation::Win32::GetProcAddress(FModule, AFunctionName.CString()));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
	return Result;
}