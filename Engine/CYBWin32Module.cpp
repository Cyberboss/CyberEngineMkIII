//! @file CYBWin32Module.cpp Implements CYB::Platform::Module for Win32

#include "CYB.hpp"

CYB::Platform::Implementation::Module::Module(const char* const AModuleName) :
	FModule(Win32::LoadLibraryA(AModuleName))
{
	if (FModule == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_LOAD_FAILURE);
}
CYB::Platform::Implementation::Module::~Module() {
	if (FModule != nullptr)
		Win32::FreeLibrary(FModule);
}

void* CYB::Platform::Module::LoadFunction(const char* const AFunctionName) {
	auto Result(Implementation::Win32::GetProcAddress(FModule, AFunctionName));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
	return Result;
}