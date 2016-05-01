//! @file CYBPosixModule.cpp Implements CYB::Platform::Module for Posix

#include "CYB.hpp"

CYB::Platform::Implementation::Module::Module(const char* const AModuleName) :
	FModule{ Posix::dlopen(AModuleName, RTLD_LAZY) }
{
	if (FModule == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_LOAD_FAILURE);
}
CYB::Platform::Implementation::Module::~Module() {
	if (FModule != nullptr)
		Posix::dlclose(FModule);
}

void* CYB::Platform::Module::LoadFunction(const char* const AFunctionName) {
	auto Result(Implementation::Posix::dlsym(FModule, AFunctionName));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
	return Result;
}