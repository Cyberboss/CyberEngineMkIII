//! @file Constructor.inl Implements CYB::API::Interop::Constructor
#pragma once

template <typename AType, typename... ATypes> CYB::API::Interop::Constructor<AType, ATypes...>::Constructor(AType&& AParam, ATypes&&... AParams, const unsigned short ASize) noexcept :
	Constructor<ATypes...>(std::forward<ATypes>(AParams)..., ASize),
	FParam(std::move(AParam))
{}

template <typename AType, typename... ATypes> CYB::API::Interop::Constructor<AType, ATypes...>::Constructor(AType&& AParam, ATypes&&... AParams) noexcept :
	Constructor<ATypes...>(std::forward<ATypes>(AParams)..., sizeof(*this)),
	FParam(std::move(AParam))
{}

template <typename AType, typename... ATypes> template <class AAllocatable, typename... AArgs> void CYB::API::Interop::Constructor<AType, ATypes...>::Construct(void* const ALocation, AArgs&&... AArguments) {
	Constructor<ATypes...>::template Construct<AAllocatable>(ALocation, std::forward<AArgs>(AArguments)..., std::move(FParam));
}

template <typename AType, typename... ATypes> template <class AAllocatable> void CYB::API::Interop::Constructor<AType, ATypes...>::Construct(void* const ALocation) {
	Constructor<ATypes...>::template Construct<AAllocatable>(ALocation, std::move(FParam));
}

template <typename AType> CYB::API::Interop::Constructor<AType>::Constructor(AType&& AParam, const unsigned short ASize) noexcept :
	Constructor<void>(ASize),
	FParam(std::move(AParam))
{}

template <typename AType> CYB::API::Interop::Constructor<AType>::Constructor(AType&& AParam) noexcept :
	Constructor<void>(sizeof(*this)),
	FParam(std::move(AParam))
{}

template <typename AType> template <class AAllocatable, typename... AArgs> void CYB::API::Interop::Constructor<AType>::Construct(void* const ALocation, AArgs&&... AArguments) {
	Constructor<void>::Construct<AAllocatable>(ALocation, std::forward<AArgs>(AArguments)..., std::move(FParam));
}

template <typename AType> template <class AAllocatable> void CYB::API::Interop::Constructor<AType>::Construct(void* const ALocation) {
	Constructor<void>::Construct<AAllocatable>(ALocation, std::move(FParam));
}

inline CYB::API::Interop::Constructor<void>::Constructor(const unsigned short ASize) noexcept :
	FSize(ASize)
{}

inline CYB::API::Interop::Constructor<void>::Constructor() noexcept :
	FSize(sizeof(*this))
{}

template <class AAllocatable, typename... AArgs> void CYB::API::Interop::Constructor<void>::Construct(void* const ALocation, AArgs&&... AArguments) {
	Allocator::InPlaceAllocation<AAllocatable>(ALocation, std::forward<AArgs>(AArguments)...);
}

template <class AAllocatable> void CYB::API::Interop::Constructor<void>::Construct(void* const ALocation) {
	Allocator::InPlaceAllocation<AAllocatable>(ALocation);
}

template <class AAllocatable> bool CYB::API::Interop::Constructor<void>::Valid(void) const noexcept {
	return std::is_base_of<Allocatable, AAllocatable>::value && FSize == sizeof(typename AAllocatable::Constructor);
}