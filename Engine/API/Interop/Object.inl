#pragma once

template <class AInterface> CYB::API::Interop::Object<AInterface>::Object(AInterface* const APointer) noexcept :
	FPointer(APointer)
{}

template <class AInterface> CYB::API::Interop::Object<AInterface>::Object(const Object<AInterface>& ACopy) :
	FPointer(Context::GetContext().FAllocator.CopyObject<AInterface>(ACopy()))
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
	Context::GetContext().FAllocator.DeleteObject<AInterface>(FPointer);
}

template <class AInterface> AInterface& CYB::API::Interop::Object<AInterface>::operator()(void) noexcept {
	return *FPointer;
}

template <class AInterface> const AInterface& CYB::API::Interop::Object<AInterface>::operator()(void) const noexcept {
	return *FPointer;
}

template <class AInterface> bool CYB::API::Interop::Object<AInterface>::Valid(void) const noexcept {
	return FPointer != nullptr;
}

template <class AInterface> template <class ANewInterface> CYB::API::Interop::Object<ANewInterface> CYB::API::Interop::Object<AInterface>::Upcast(Object<AInterface>&& AUpcastable) noexcept {
	static_assert(std::is_base_of<ANewInterface, AInterface>::value, "Attempted to upcast to non-base class");
	auto Pointer(AUpcastable.FPointer);
	AUpcastable.FPointer = nullptr;
	return Object<ANewInterface>(static_cast<ANewInterface*>(Pointer));
}