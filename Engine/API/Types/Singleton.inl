//! @file Singleton.inl Implements functions from CYB::API::Singleton
#pragma once
template <class AParent> AParent* CYB::API::Singleton<AParent>::FSingleton(nullptr);

template <class AParent> CYB::API::Singleton<AParent>::Singleton() noexcept {
	static_assert(std::is_base_of<Singleton<AParent>, AParent>::value, "CYB::API::Singleton may only be used via inheritance");
	Assert::Equal<AParent*>(FSingleton, nullptr);
	FSingleton = static_cast<AParent*>(this);
}
template <class AParent> CYB::API::Singleton<AParent>::~Singleton() {
	Assert::Equal(FSingleton, static_cast<AParent*>(this));
	FSingleton = nullptr;
}
#ifdef CYB_BUILDING_TESTS
template <class AParent> void CYB::API::Singleton<AParent>::SetTestPointer(void* const APointer) {
	FSingleton = static_cast<AParent*>(APointer);
}
#endif