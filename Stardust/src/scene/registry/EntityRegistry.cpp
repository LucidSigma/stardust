#include "stardust/scene/registry/EntityRegistry.h"

namespace stardust
{
    [[nodiscard]] Entity stardust::EntityRegistry::CreateEntity(Scene& scene)
    {
        return Entity(m_handle.create(), scene);
    }

    void EntityRegistry::DestroyEntity(const Entity& entity)
    {
        m_handle.destroy(entity.GetHandle());
    }

    void EntityRegistry::ClearAllEntities()
    {
        m_handle.clear();
    }
}