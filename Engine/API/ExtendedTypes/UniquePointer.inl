#pragma once

template <typename AType> typename CYB::API::UniquePointer<AType>::BaseType CYB::API::UniquePointer<AType>::InitBase(AType* const APointer, const std::false_type AIsDestructible) noexcept {
	static_cast<void>(AIsDestructible);
	return BaseType(APointer, Deallocate);
}

template <typename AType> typename CYB::API::UniquePointer<AType>::BaseType CYB::API::UniquePointer<AType>::InitBase(AType* const APointer, const std::true_type AIsDestructible) noexcept {
	static_cast<void>(AIsDestructible);
	return BaseType(APointer, Delete);
}

template <typename AType> void CYB::API::UniquePointer<AType>::Deallocate(AType* const APointer) noexcept {
	Context().FAllocator.FHeap.Free(APointer);
}

template <typename AType> void CYB::API::UniquePointer<AType>::Delete(AType* const APointer) noexcept {
	Context().FAllocator.DeleteObject(APointer);
}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer() noexcept :
	BaseType(InitBase(nullptr, std::is_destructible<AType>::type()))
{}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer(const std::nullptr_t AIgnored) noexcept :
	BaseType(InitBase(nullptr, std::is_destructible<AType>::type()))
{
	static_cast<void>(AIgnored);
}

template <typename AType> CYB::API::UniquePointer<AType>::UniquePointer(AType* const APointer) noexcept :
	BaseType(InitBase(APointer, std::is_destructible<AType>::type()))
{}