#pragma once

template <class AInterface> CYB::API::Object<AInterface>::Object(AInterface* const APointer) :
	FPointer(APointer)
{}

template <class AInterface> CYB::API::Object<AInterface>::Object(Object<AInterface>&& AMove) :
	FPointer(AMove.FPointer)
{
	AMove.FPointer = nullptr;
}

template <class AInterface> CYB::API::Object<AInterface>& CYB::API::Object<AInterface>::operator=(Object<AInterface>&& AMove) {
	FPointer = AMove.FPointer;
	AMove.FPointer = nullptr;
	return *this;
}

template <class AInterface> CYB::API::Object<AInterface>::~Object() {
	if (FPointer != nullptr) {
		FPointer->~AInterface();
		Allocator::GetAllocator().FHeap.Free(FPointer);
	}
}

template <class AInterface> AInterface& CYB::API::Object<AInterface>::operator()(void) {
	return *FPointer;
}

template <class AInterface> const AInterface& CYB::API::Object<AInterface>::operator()(void) const {
	return *FPointer;
}