#include "stardust/ecs/entity/Entity.h"

namespace stardust
{
    Entity::Entity(const EntityHandle handle, Scene& scene)
        : m_handle(handle), m_registry(&scene.GetEntityRegistry())
    { }

    auto Entity::Destroy() noexcept -> void
    {
        if (IsValid())
        {
            m_registry->DestroyEntity(*this);
            m_handle = NullEntityHandle;
        }
    }

    [[nodiscard]] auto Entity::IsOrphan() const -> bool
    {
        return m_registry->GetHandle().orphan(m_handle);
    }
}
