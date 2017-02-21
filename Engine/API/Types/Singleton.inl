//! @file Singleton.inl Implements functions from CYB::API::Singleton
#pragma once
template <class AParent> AParent* CYB::API::Singleton<AParent>::FSingleton(nullptr);

template <class AParent> CYB::API::Singleton<AParent>::Singleton(const bool AInit) noexcept {
	static_assert(std::is_base_of<Singleton<AParent>, AParent>::value, "CYB::API::Singleton may only be used via inheritance");
	if (AInit) {
		Assert::Equal<AParent*>(FSingleton, nullptr);
		FSingleton = static_cast<AParent*>(this);
	}
}
template <class AParent> CYB::API::Singleton<AParent>::~Singleton() {
	Assert::Equal(FSingleton, static_cast<AParent*>(this));
	FSingleton = nullptr;
}