#pragma once

template <typename AType> void CYB::API::UniquePointer<AType>::Delete(AType* const APointer) noexcept {
	//! @todo Make this use Allocator::Delete after the refactor
	APointer->~AType();
	Context().FAllocator.FHeap.Free(APointer);
}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer() noexcept :
	unique_ptr<AType, void(*)(AType* const)>(nullptr, Delete)
{}

template <typename AType> constexpr CYB::API::UniquePointer<AType>::UniquePointer(const std::nullptr_t AIgnored) noexcept :
	unique_ptr<AType, void(*)(AType* const)>(nullptr, Delete)
{
	static_cast<void>(AIgnored);
}

template <typename AType> CYB::API::UniquePointer<AType>::UniquePointer(AType* const APointer) noexcept :
	unique_ptr<AType, void(*)(AType* const)>(APointer, Delete)
{}