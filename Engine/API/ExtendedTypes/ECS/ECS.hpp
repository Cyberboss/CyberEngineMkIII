//! @file ECS.hpp The entity component system for the engine

namespace CYB {
	namespace API {
		//! @brief Contains classes relating to the entity component system
		namespace ECS {
			class Entity final {
				friend class World;
			private:
				World& FWorld;	//!< @brief Reference to the world this entity exists in
				unsigned int FID;	//!< @brief Assigned ID of the entity
				
				/*!
					@brief Initialize an Entity and queue the EntityCreated event to listeners
					@param AID The Entity's identifier in the world
					@param AWorld The World this Entity belongs to
					@par Thread Safety
						This function requires no thread safety
				*/
				Entity(const unsigned int AID, World& AWorld) noexcept;
			public:
				//! @brief See @ref interstructors
				Entity(const Entity&) = default;
				
				/*!
					@brief Associate a Component with this Entity and potentially queue the ComponentAdded event to listeners
					@tparam AComponent The Component being associated
					@param AArgs The arguments to the constructor of AComponent
					@return A pointer to the Component if it was not already associated with the Entity and the the ComponentAdded event was queued, nullptr otherwise
					@par Thread Safety
						???
				*/
				template <typename AComponent, typename... AConstructorArgs> AComponent* AddComponent(AConstructorArgs&&... AArgs);
				/*!
					@brief Unassociates a Component with this Entity and potentially queue the ComponentRemoved event to listeners
					@tparam AComponent The Component being unassociated
					@return True if the AComponent was already associated with the Entity, false otherwise
					@par Thread Safety
						???
				*/
				template <typename AComponent> bool RemoveComponent(void);
				
				/*!
					@brief Get a Component associated with this Entity
					@tparam AComponent The Component being retrieved
					@return A pointer to the Component if it is associated with the Entity, nullptr otherwise
					@par Thread Safety
						???
				*/
				template <typename AComponent> AComponent* GetComponent(void);
				
				/*!
					@brief Invalidate an Entity and queue EntityDestroyed event to listeners
					@par Thread Safety
						???
				*/
				void Destroy(void) noexcept;
			};

			/*!
				@brief A data container for Entitys. Derived classes should be POD types and should not implement behaviour
				@attention A Component type cannot be in use in multiple worlds simultaneously
			*/
			template <typename ADataClass> class Component {
				static_assert(std::is_default_constructible<ADataClass>::value, "Component must be default constructible");
				static_assert(std::is_final<ADataClass>::value, "Component must be final");
				friend class World;
				friend class Entity;
			private:
				static unsigned int FID;	//!< @brief The ID associated with the component in the World
			};

			//! @brief Manager for a group of Entitys and Components
			class World : public Interop::Allocatable {
				friend class Entity;
			private:
				/*!
					@brief Register a Component with an ID. Necessary before use within the World
					@param AComponentID A reference to the Component's FID field
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				virtual void RegisterComponent(unsigned int& AComponentID) noexcept = 0;

				/*!
					@brief Allocate a Component of type @p AComponentID for Entity @p AEntityID
					@param AEntityID The ID of the Entity to allocate the Component for
					@param AComponentID The ID of the Component being allocated
					@param ASize The size of the Component represented by @p AComponentID
					@return A pointer to the allocated Component if it did not already exist in the Entity, nullptr otherwise
					@par Thread Safety
						???
				*/
				virtual void* AllocateComponent(const unsigned int AEntityID, const unsigned int AComponentID, const unsigned long long ASize) = 0;
				/*!
					@brief Deallocate a Component of type @p AComponentID from Entity @p AEntityID
					@param AEntityID The ID of the Entity to deallocate the Component from
					@param AComponentID The ID of the Component being deallocated
					@return True if the Component had been previously allocated for the Entity, false otherwise
					@par Thread Safety
						???
				*/
				virtual bool DeallocateComponent(const unsigned int AEntityID, const unsigned int AComponentID) noexcept = 0;

				/*!
					@brief Retrieve a Component of type @p AComponentID from Entity @p AEntityID
					@param AEntityID The ID of the Entity to retrieve the Component from
					@param AComponentID The ID of the Component being retrieved
					@return A pointer to the allocated Component if it exists in the Entity, nullptr otherwise
					@par Thread Safety
						???
				*/
				virtual void* GetComponent(const unsigned int AEntityID, const unsigned int AComponentID) noexcept = 0;

				/*!
					@brief Deallocate and stop processing of an Entity and it's Components
					@param AID The ID of the Entity to destroy
					@par Thread Safety
						???
				*/
				virtual void DestroyEntity(const unsigned int AID) noexcept = 0;
			public:
				/*!
					@brief Register a Component with an ID. Necessary before use within the World
					@tparam AComponent The Component being registered
					@par Thread Safety
						This function requires synchronization at the object level
				*/
				template <typename AComponent> void RegisterComponent(void);

				/*!
					@brief Allocate and start processing an Entity with no Components
					@return A new, empty Entity
					@par Thread Safety
						???
				*/
				virtual Entity CreateEntity(void) = 0;
			};
		};
	};
};

CYB_SET_ALLOCATABLE_ID_EX(ECS, World);

#include "ECS.inl"