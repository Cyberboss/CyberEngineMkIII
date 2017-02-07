#pragma once

template <class AObject, typename... AArgs> AObject* CYB::Engine::Allocator::RawObject(AArgs&&... AArguments) {
	API::UniquePointer<void> Buf(FHeap.Alloc(sizeof(AObject)));
	auto Result(InPlaceAllocation<AObject>(Buf.get(), std::forward<AArgs>(AArguments)...));
	Buf.release();
	return Result;
}
