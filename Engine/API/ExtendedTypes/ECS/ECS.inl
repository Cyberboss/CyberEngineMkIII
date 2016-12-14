//! @file ECS.inl Implements the entity component system

template <typename ADataClass> unsigned int CYB::API::ECS::Component<ADataClass>::FID(0);

inline CYB::API::ECS::Entity::Entity(const unsigned int AID, World& AWorld) noexcept :
	FWorld(AWorld),
	FID(AID)
{}

template <typename AComponent, typename... AConstructorArgs> AComponent* CYB::API::ECS::Entity::AddComponent(AConstructorArgs&&... AArgs) {
	static_assert(std::is_base_of<AComponent, Component>::value, "Attempted to add a non-component");
	auto ComponentMemory(FWorld.AllocateComponent(FID, AComponent::FID, sizeof(AComponent)));
	if (Memory == nullptr)
		return nullptr;
	return new (ComponentMemory) AComponent(std::forward<AConstructorArgs>(AArgs)...);
}

template <typename AComponent> bool CYB::API::ECS::Entity::RemoveComponent(void) {
	static_assert(std::is_base_of<AComponent, Component>::value, "Attempted to remove a non-component");
	return FWorld.DeallocateComponent(FID, AComponent::FID);
}

template <typename AComponent> AComponent* CYB::API::ECS::Entity::GetComponent(void) {
	static_assert(std::is_base_of<AComponent, Component>::value , "Attempted to remove a non-component");
	return static_cast<AComponent*>(FWorld.GetComponent(FID, AComponent::FID));
}

inline void CYB::API::ECS::Entity::Destroy(void) noexcept {
	FWorld.DestroyEntity(FID);
	FID = 0;
}

template <typename AComponent> void CYB::API::ECS::World::RegisterComponent(void) {
	static_assert(std::is_base_of<AComponent, Component>::value, "Attempted to register non-component");

	RegisterComponent(AComponent::FID);
}