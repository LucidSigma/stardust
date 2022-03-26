#include "stardust/ecs/registry/EntityRegistry.h"

#include "stardust/ecs/bundle/EntityBundle.h"
#include "stardust/ecs/entity/Entity.h"
#include "stardust/scene/Scene.h"

namespace stardust
{
    [[nodiscard]] auto EntityRegistry::CreateEntity(Scene& scene, const ObserverPointer<EntityBundle> entityBundle) -> Entity
    {
        if (entityBundle != nullptr)
        {
            return entityBundle->CreateEntity();
        }
        else
        {
            return Entity(m_handle.create(), scene);
        }
    }

    auto EntityRegistry::DestroyEntity(const Entity& entity) -> void
    {
        m_handle.destroy(entity.GetHandle());
    }

    auto EntityRegistry::ClearAllEntities() noexcept -> void
    {
        m_handle.clear();
    }
}
