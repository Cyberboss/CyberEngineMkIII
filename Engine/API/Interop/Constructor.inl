//! @file Constructor.inl Implements CYB::API::Interop::Constructor
#pragma once

template <typename AType, typename... ATypes> CYB::API::Interop::Constructor<AType, ATypes...>::Constructor(AType&& AParam, ATypes&&... AParams, const unsigned short ASize) noexcept :
	Constructor<ATypes...>(std::forward(AParams)..., ASize),
	FParam(std::move(AParam))
{}

template <typename AType, typename... ATypes> CYB::API::Interop::Constructor<AType, ATypes...>::Constructor(AType&& AParam, ATypes&&... AParams) noexcept :
	Constructor<ATypes...>(std::forward(AParams)..., sizeof(*this)),
	FParam(std::move(AParam))
{}

template <typename AType> CYB::API::Interop::Constructor<AType>::Constructor(AType&& AParam, const unsigned short ASize) noexcept :
	Constructor<void>(ASize),
	FParam(std::move(AParam))
{}

template <typename AType> CYB::API::Interop::Constructor<AType>::Constructor(AType&& AParam) noexcept :
	Constructor<void>(sizeof(*this)),
	FParam(std::move(AParam))
{}

inline CYB::API::Interop::Constructor<void>::Constructor(const unsigned short ASize) noexcept :
	FSize(ASize)
{}

inline CYB::API::Interop::Constructor<void>::Constructor() noexcept :
	FSize(sizeof(*this))
{}

template <class AAllocatable> bool CYB::API::Interop::Constructor<void>::Valid(void) const noexcept {
	return std::is_base_of<Allocatable, AAllocatable>::value && FSize == sizeof(AAllocatable::Constructor);
}