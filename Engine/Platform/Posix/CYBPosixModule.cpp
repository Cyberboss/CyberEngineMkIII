#include "CYB.hpp"

using namespace CYB::Platform::Implementation::Posix;

#include <cstring>

CYB::Platform::Implementation::Module::Module(const API::String::CStyle& AModuleName) {
	char PrefixAndBuffer[256](u8"lib");
	auto const PostPrefixOffset(PrefixAndBuffer + 3);

	memcpy(PostPrefixOffset, AModuleName.CString(), AModuleName.RawLength());
	memcpy(PostPrefixOffset + AModuleName.RawLength(), Suffix(), API::String::Static(Suffix()).RawLength());

	FModule = Posix::dlopen(PrefixAndBuffer, RTLD_LAZY);
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