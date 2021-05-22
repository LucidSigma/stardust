#include "stardust/scene/registry/EntityRegistry.h"

#include "stardust/scene/entity/Entity.h"
#include "stardust/scene/Scene.h"

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

    [[nodiscard]] bool EntityRegistry::IsEntityValid(const Entity& entity) const
    {
        return m_handle.valid(entity.GetHandle());
    }

    void EntityRegistry::ClearAllEntities()
    {
        m_handle.clear();
    }
}