#pragma once

template <typename AType> void CYB::API::UniquePointer<AType>::Delete(AType* const APointer) noexcept {
	Context().FAllocator.DeleteObject(APointer);
}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer() noexcept :
	BaseType(nullptr, Delete)
{}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer(const std::nullptr_t AIgnored) noexcept :
	BaseType(nullptr, Delete)
{
	static_cast<void>(AIgnored);
}

template <typename AType> CYB::API::UniquePointer<AType>::UniquePointer(AType* const APointer) noexcept :
	BaseType(APointer, Delete)
{}