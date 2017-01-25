#include "TestHeader.hpp"
#ifndef TARGET_OS_WINDOWS
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif

bool BasicCmp(const char* const A, const char* const B) {
	return CYB::API::String::Static(A) == CYB::API::String::Static(B);
}

template <> template <> void CYB::API::Singleton<CYB::Engine::Core>::Backdoor<void*>(void*& AHooker) {
	FSingleton = static_cast<CYB::Engine::Core*>(AHooker);
}
template <> void CYB::Engine::Core::Backdoor<Fake::Core>(Fake::Core& AHooker) {
	new (&(reinterpret_cast<Core*>(AHooker.FBytes)->FEngineContext)) CYB::Engine::Context(AHooker.FHeap, AHooker.FLogger, true);	//this hurts you
}

Fake::Logger::Logger() :
	FLogPath(u8"Catch test suite")
{}

void Fake::Logger::Log(const CYB::API::String::CStyle& AMessage, const Level ALevel) {
	static_cast<void>(AMessage);
	static_cast<void>(ALevel);
}

void Fake::Logger::Flush(void) const noexcept {}

const CYB::API::String::CStyle& Fake::Logger::CurrentLog(void) const noexcept {
	return FLogPath;
}

void Fake::Logger::SetDebugLogging(const bool AIgnored) noexcept {
	static_cast<void>(AIgnored);
}

Fake::Core::Core() {
	CYB::Engine::Core::Backdoor(*this);
	ResetToFakeCorePointer();
}

Fake::Core FFakeCore;

void Fake::Core::ResetToFakeCorePointer(void) {
	auto ref(static_cast<void*>(FFakeCore.FBytes));
	CYB::API::Singleton<CYB::Engine::Core>::Backdoor(ref);
	CYB::Engine::Core::GetCore().DefaultContext();
}

void Fake::Core::NullifySingleton(void) {
	void* Null(nullptr);
	CYB::API::Singleton<CYB::Engine::Core>::Backdoor(Null);
}

void* Fake::Heap::Alloc(const int AAmount) {
	return malloc(static_cast<size_t>(AAmount));
}
void* Fake::Heap::Realloc(void* const AOld, const int ANewSize) {
	return realloc(AOld, static_cast<size_t>(ANewSize));
}
void Fake::Heap::Free(void* const AOld) noexcept {
	free(AOld);
}

static bool _CallRedirected(const CYB::Platform::System::Sys::CallNumber ACallNumber) noexcept {
	for (auto Current(Fake::SysCalls::FMapList); Current != nullptr; Current = Current->FNext)
		if (Current->FCallNumber == ACallNumber)
			return true;
	return false;
}

bool CYB::Platform::System::Sys::CallRedirected(const CallNumber ACallNumber) noexcept {
	return _CallRedirected(ACallNumber);
}

Fake::SysCalls::MapList* Fake::SysCalls::FMapList(nullptr);

unsigned long long CYB::Platform::System::Sys::RedirectedCall(const CallNumber ACallNumber, const Union64 AArg1, const Union64 AArg2, const Union64 AArg3, const Union64 AArg4, const Union64 AArg5, const Union64 AArg6) {
	Fake::SysCalls::Args Args{ AArg1, AArg2, AArg3, AArg4, AArg5, AArg6 };
	for (auto Current(Fake::SysCalls::FMapList); Current != nullptr; Current = Current->FNext)
		if (Current->FCallNumber == ACallNumber)
			return Current->FFunction(Args);
	API::Assert::HCF();
}

void Fake::SysCalls::OverrideCall(const CYB::Platform::System::Sys::CallNumber ACallNumber, CallPointer ANewFunction) {
	CYB::API::Assert::False(_CallRedirected(ACallNumber));
	FMapList = new MapList{ FMapList, ANewFunction, ACallNumber };
}

void Fake::SysCalls::ResetCall(const CYB::Platform::System::Sys::CallNumber ACallNumber) {
	CYB::API::Assert::True(_CallRedirected(ACallNumber));
	for (MapList* Current(FMapList), *Previous(nullptr); Current != nullptr; Previous = Current, Current = Current->FNext)
		if (Current->FCallNumber == ACallNumber) {
			if (Previous == nullptr)
				FMapList = Current->FNext;
			else
				Previous->FNext = Current->FNext;
			delete Current;
			break;
		}
}

SysCallOverride::SysCallOverride(const CYB::Platform::System::Sys::CallNumber ACallNumber, Fake::SysCalls::CallPointer ANewFunction):
	FCallNumber(ACallNumber)
{
	Fake::SysCalls::OverrideCall(ACallNumber, ANewFunction);
}
SysCallOverride::~SysCallOverride() {
	Fake::SysCalls::ResetCall(FCallNumber);
}

void TestLog(const char* ALog) {
	WARN(ALog);
}
void TestLogNum(const long long ALog) {
	WARN(ALog);
}

unsigned int lastError;
std::deque<unsigned int> NextError;

REDIRECTED_FUNCTION(ErrorCallOverride) {
	if (!NextError.empty()) {
		lastError = NextError.front();
		NextError.pop_front();
	}
	return lastError;
}

void PushError(unsigned int AError) {
	NextError.push_back(AError);
	errno = static_cast<int>(AError);
}

CallRedirect<CYB::Platform::Modules::Kernel32::FAutoModule, CYB::Platform::Modules::Kernel32::GetLastError> OverrideError(ModuleDependancy<CYB::Platform::Modules::Kernel32>& AModule, unsigned int AErrorCode) {
	PushError(AErrorCode);
	return AModule.Redirect<CYB::Platform::Modules::Kernel32::GetLastError, ErrorCallOverride>();
}