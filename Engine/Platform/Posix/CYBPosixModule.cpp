#include "CYB.hpp"

using namespace CYB::Platform::Implementation::Posix;

#include <cstring>

CYB::Platform::Implementation::Module::Module(const API::String::CStyle& AModuleName) {
	//This can be called without Core
	FModule = Posix::dlopen(AModuleName.CString(), RTLD_LAZY);
	if (FModule == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_LOAD_FAILURE);
}
CYB::Platform::Implementation::Module::~Module() {
	if (FModule != nullptr)
		//This can be called without Core
		Posix::dlclose(FModule);
}

void* CYB::Platform::Module::LoadFunction(const API::String::CStyle& AFunctionName) {
	//This can be called without Core
	auto Result(Implementation::Posix::dlsym(FModule, AFunctionName.CString()));
	if (Result == nullptr)
		throw Exception::SystemData(Exception::SystemData::MODULE_FUNCTION_LOAD_FAILURE);
	return Result;
}