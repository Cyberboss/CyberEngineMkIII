#include "TestHeader.hpp"

FakeHeap::FakeHeap(const unsigned long long ASize) {
	CYB::Core().FHeap = CYB::Engine::Memory::Heap(ASize);
}
FakeHeap::~FakeHeap() {
	CYB::Core().FHeap.~Heap();
}

FakeCore FFakeCore;
