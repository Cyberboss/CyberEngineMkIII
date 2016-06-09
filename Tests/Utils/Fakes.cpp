#include "TestHeader.hpp"
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

Fake::Core::Core(const unsigned long long ASize) {
	CYB::Core().SetTestPointer(FBytes);
	static_cast<void>(ASize);
	//K32 dependancy
//	new (&CYB::Core().FHeap) CYB::Engine::Memory::Heap(ASize);
	FAllocator = new Allocator();
}
Fake::Core::~Core() {
	delete FAllocator;
}

Fake::Core FFakeCore(CYB::API::ByteConverters::Megabytes(50));

void* Fake::Heap::Alloc(const int AAmount) {
	return malloc(static_cast<size_t>(AAmount));
}
void* Fake::Heap::Realloc(void* const AOld, const int ANewSize) {
	return realloc(AOld, static_cast<size_t>(ANewSize));
}
void Fake::Heap::Free(void* const AOld) {
	free(AOld);
}