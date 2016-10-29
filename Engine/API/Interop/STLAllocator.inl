#pragma once

template <typename AType> auto CYB::API::Interop::STLAllocator<AType>::address(reference AObject) noexcept -> pointer {
	return std::addressof<AType>(AObject);
}

template <typename AType> auto CYB::API::Interop::STLAllocator<AType>::address(const_reference AObject) noexcept -> const_pointer {
	return std::addressof<AType>(AObject);
}

template <typename AType> AType* CYB::API::Interop::STLAllocator<AType>::allocate(const std::size_t ACount) {
	const auto Size(sizeof(AType) * ACount);
	Assert::LessThanOrEqual<unsigned long long>(Size, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	return static_cast<AType*>(Context::GetContext().FAllocator.FHeap.Alloc(static_cast<int>(Size)));
}

template <typename AType> void CYB::API::Interop::STLAllocator<AType>::deallocate(AType* const AObject, const std::size_t ACount) noexcept {
	const auto Size(sizeof(AType) * ACount);
	Assert::LessThanOrEqual<unsigned long long>(Size, static_cast<unsigned long long>(std::numeric_limits<int>::max()));
	Context::GetContext().FAllocator.FHeap.Free(AObject);
}

template <typename AType> template <typename AOther, typename... AArgs> void CYB::API::Interop::STLAllocator<AType>::construct(AOther* const APointer, AArgs&&... AArguments) {
	::new (static_cast<void*>(APointer)) AOther(std::forward<AArgs>(AArguments)...);
}

template <typename AType> template <typename AOther> void CYB::API::Interop::STLAllocator<AType>::destroy(AOther* const APointer) noexcept {
	static_cast<void>(APointer);	//MSVC thinks calling the destructor counts as having the pointer being unreferenced
	APointer->~AOther();
}

template <typename AType> template <typename AOtherType> CYB::API::Interop::STLAllocator<AType>::STLAllocator(const AOtherType& AOther) noexcept {
	static_cast<void>(AOther);
}

template <typename AType> template<typename ARType> constexpr bool CYB::API::Interop::STLAllocator<AType>::operator==(const STLAllocator<ARType>& ARHS) const noexcept {
	static_cast<void>(ARHS);
	return true;
}

template <typename AType> template<typename ARType> constexpr bool CYB::API::Interop::STLAllocator<AType>::operator!=(const STLAllocator<ARType>& ARHS) const noexcept {
	static_cast<void>(ARHS);
	return false;
}