#include "CYB.hpp"

CYB::Platform::Modules::Implementation::Module::Module(const CYB::API::String::CStyle& AModuleName) :
	//No FK32 because this can be called without Core
	FModule(reinterpret_cast<Win32::HMODULE>(Platform::System::Sys::Call(Platform::System::Sys::LOAD_LIBRARY, const_cast<char*>(AModuleName.CString()))))
{
	if (FModule == nullptr)
		throw Exception::Internal(Exception::Internal::MODULE_LOAD_FAILURE);
}
CYB::Platform::Modules::Implementation::Module::~Module() {
	if (FModule != nullptr)
		//No FK32 because this can be called without Core
		Platform::System::Sys::Call(Platform::System::Sys::CLOSE_LIBRARY, FModule);
}

void* CYB::Platform::Modules::Module::LoadFunction(const CYB::API::String::CStyle& AFunctionName) {
	auto Result(reinterpret_cast<void*>(Platform::System::Sys::Call(Platform::System::Sys::LOAD_SYMBOL, FModule, const_cast<char*>(AFunctionName.CString()))));
	if (Result == nullptr)
		throw Exception::Internal(Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE);
	return Result;
}