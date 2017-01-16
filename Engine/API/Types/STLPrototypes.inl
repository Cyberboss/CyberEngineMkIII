#pragma once

template <typename AType> void CYB::API::UniquePointer<AType>::Delete(AType* const APointer) noexcept {
	Context().FAllocator.Delete(APointer);
}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer() noexcept :
	unique_ptr(nullptr, Delete)
{}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer(const nullptr_t AIgnored) noexcept :
	unique_ptr(nullptr, Delete)
{}

template <typename AType> CYB::API::UniquePointer<AType>::UniquePointer(pointer const APointer) noexcept :
	unique_ptr(APointer, Delete)
{}