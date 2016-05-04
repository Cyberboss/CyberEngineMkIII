//! @file CYBPosixModule.cpp Implements CYB::Platform::Module for Posix

#include "CYB.hpp"

CYB::Platform::Implementation::Module::Module(const API::String::CStyle& AModuleName) :
	FModule{ Posix::dlopen(AModuleName.CString(), RTLD_LAZY) }
{
	if (FModule == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_LOAD_FAILURE);
}
CYB::Platform::Implementation::Module::~Module() {
	if (FModule != nullptr)
		Posix::dlclose(FModule);
}

void* CYB::Platform::Module::LoadFunction(const API::String::CStyle& AFunctionName) {
	auto Result(Implementation::Posix::dlsym(FModule, AFunctionName.CString()));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
	return Result;
}