#pragma once

template <typename AType> void CYB::API::UniquePointer<AType>::Delete(AType* const APointer) noexcept {
	Context().FAllocator.Delete(APointer);
}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer() noexcept :
std::unique_ptr(nullptr, Delete)
{}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer(const std::nullptr_t AIgnored) noexcept :
	std::unique_ptr(nullptr, Delete)
{}

template <typename AType> CYB::API::UniquePointer<AType>::UniquePointer(pointer const APointer) noexcept :
	std::unique_ptr(APointer, Delete)
{}