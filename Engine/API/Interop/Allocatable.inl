//! @file Allocatable.inl Defines the base of CYB::API::Interop::Allocatable::GetID;
#pragma once
template <class AParent> constexpr CYB::API::Interop::Allocatable::ID CYB::API::Interop::Allocatable::GetID(void) {
	return ID::NULL_ID;
}