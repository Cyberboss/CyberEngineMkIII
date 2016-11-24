//! @file CYBPosixModule.cpp Implements CYB::Platform::Modules::Module for Posix
#include "CYB.hpp"

using namespace CYB::Platform::Posix;

#include <cstring>

CYB::Platform::Modules::Implementation::Module::Module(const API::String::CStyle& AModuleName) 
{	FModule = reinterpret_cast<void*>(Platform::System::Sys::Call(Platform::System::Sys::LOAD_LIBRARY, const_cast<char*>(AModuleName.CString()), RTLD_LAZY));	//This can be called without Core
	if (FModule == nullptr)
		throw Exception::Internal(Exception::Internal::MODULE_LOAD_FAILURE);
}
CYB::Platform::Modules::Implementation::Module::~Module() 
{	if (FModule != nullptr)
		//This can be called without Core
		Platform::System::Sys::Call(Platform::System::Sys::CLOSE_LIBRARY, FModule);
}

void* CYB::Platform::Modules::Module::LoadFunction(const API::String::CStyle& AFunctionName) 
{
	//This can be called without Core
	auto Result(reinterpret_cast<void*>(Platform::System::Sys::Call(Platform::System::Sys::LOAD_SYMBOL, FModule, const_cast<char*>(AFunctionName.CString()))));
	if (Result == nullptr)
		throw Exception::Internal(Exception::Internal::MODULE_FUNCTION_LOAD_FAILURE);
	return Result; 
}