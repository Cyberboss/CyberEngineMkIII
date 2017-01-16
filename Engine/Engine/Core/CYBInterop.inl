#pragma once

template <class AObject, typename... AArgs> AObject* CYB::Engine::Allocator::RawObject(AArgs&&... AArguments) {
	class AutoFreeBuffer {
	public:
		Allocator& FAllocator;
		void* FBuffer;
	public:
		AutoFreeBuffer(void* const ABuffer, Allocator& AAllocator) :
			FAllocator(AAllocator),
			FBuffer(ABuffer)
		{}
		~AutoFreeBuffer() {
			FAllocator.FHeap.Free(FBuffer);
		}
	};

	AutoFreeBuffer Buf(FHeap.Alloc(sizeof(AObject)), *this);
	using namespace std;
	auto Result(InPlaceAllocation<AObject>(Buf.FBuffer, std::forward<AArgs>(AArguments)...));
	Buf.FBuffer = nullptr;
	return Result;
}
