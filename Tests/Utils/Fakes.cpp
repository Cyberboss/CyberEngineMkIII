#include "TestHeader.hpp"
#ifndef TARGET_OS_WINDOWS
#pragma clang diagnostic ignored "-Wdelete-non-virtual-dtor"
#endif

CYB::API::Interop::Allocator* CYB::API::Interop::Allocator::FAllocator;

Fake::Allocator::Allocator() :
	CYB::API::Interop::Allocator(*new Heap())
{}
Fake::Allocator::~Allocator() {
	delete &FHeap;
}

CYB::API::Interop::Allocatable* Fake::Allocator::NewObject(const CYB::API::Interop::Allocatable::ID, const CYB::API::Interop::EmptyConstructor&) {
	CYB::API::Assert::HCF();
}

template<> template<> void CYB::API::Singleton<CYB::Engine::Core>::Backdoor<void*>(void*& AHooker) {
	FSingleton = static_cast<CYB::Engine::Core*>(AHooker);
}

Fake::Core::Core(const unsigned long long ASize) {
	ResetToFakeCorePointer();
	static_cast<void>(ASize);
	//K32 dependancy
//	new (&CYB::Core().FHeap) CYB::Engine::Memory::Heap(ASize);
	FAllocator = new Allocator();
}
Fake::Core::~Core() {
	delete FAllocator;
}

Fake::Core FFakeCore(CYB::API::ByteConverters::Megabytes(50));

void Fake::Core::ResetToFakeCorePointer(void) {
	auto ref(static_cast<void*>(FFakeCore.FBytes));
	CYB::API::Singleton<CYB::Engine::Core>::Backdoor<void*>(ref);
}

void* Fake::Heap::Alloc(const int AAmount) {
	return malloc(static_cast<size_t>(AAmount));
}
void* Fake::Heap::Realloc(void* const AOld, const int ANewSize) {
	return realloc(AOld, static_cast<size_t>(ANewSize));
}
void Fake::Heap::Free(void* const AOld) {
	free(AOld);
}