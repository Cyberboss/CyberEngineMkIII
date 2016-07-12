#pragma once

template <class AType> AType* CYB::API::Interop::STLAllocator<AType>::allocate(const std::size_t ACount) {
	const auto Size(sizeof(AType) * ACount);
	Assert::LessThanOrEqual<unsigned long long>(Size, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	return static_cast<AType*>(Allocator::GetAllocator().FHeap.Alloc(static_cast<int>(Size)));
}

template <class AType> void CYB::API::Interop::STLAllocator<AType>::deallocate(AType* const AObject, const std::size_t ACount) noexcept {
	const auto Size(sizeof(AType) * ACount);
	Assert::LessThanOrEqual<unsigned long long>(Size, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	Allocator::GetAllocator().FHeap.Free(AObject);
}

template <class AType> template <class AOtherType> CYB::API::Interop::STLAllocator<AType>::STLAllocator(const AOtherType& AOther) noexcept {
	static_cast<void>(AOther);
}

template <class AType> template<class ARType> constexpr bool CYB::API::Interop::STLAllocator<AType>::operator==(const STLAllocator<ARType>& ARHS) const noexcept {
	return true;
}

template <class AType> template<class ARType> constexpr bool CYB::API::Interop::STLAllocator<AType>::operator!=(const STLAllocator<ARType>& ARHS) const noexcept {
	return false;
}