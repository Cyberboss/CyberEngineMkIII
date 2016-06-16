#pragma once

template <class AInterface> CYB::API::Interop::Object<AInterface>::Object(AInterface* const APointer) noexcept :
	FPointer(APointer)
{}

template <class AInterface> CYB::API::Interop::Object<AInterface>::Object(Object<AInterface>&& AMove) noexcept :
	FPointer(AMove.FPointer)
{
	AMove.FPointer = nullptr;
}

template <class AInterface> CYB::API::Interop::Object<AInterface>& CYB::API::Interop::Object<AInterface>::operator=(Object<AInterface>&& AMove) noexcept {
	FPointer = AMove.FPointer;
	AMove.FPointer = nullptr;
	return *this;
}

template <class AInterface> CYB::API::Interop::Object<AInterface>::~Object() {
	if (FPointer != nullptr) {
		FPointer->~AInterface();
		Allocator::GetAllocator().FHeap.Free(FPointer);
	}
}

template <class AInterface> AInterface& CYB::API::Interop::Object<AInterface>::operator()(void) noexcept {
	return *FPointer;
}

template <class AInterface> const AInterface& CYB::API::Interop::Object<AInterface>::operator()(void) const noexcept {
	return *FPointer;
}