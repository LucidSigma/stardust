#pragma once
#ifndef STARDUST_ENTITY_H
#define STARDUST_ENTITY_H

#include <utility>

#include <entt/entt.hpp>

#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/ecs/registry/EntityRegistry.h"
#include "stardust/scene/Scene.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    class Entity final
    {
    private:
        ObserverPointer<EntityRegistry> m_registry = nullptr;
        EntityHandle m_handle = NullEntityHandle;

    public:
        Entity(const EntityHandle handle, Scene& scene);

        template <typename Component, typename... Args>
        auto AddComponent(Args&&... args) -> void
        {
            m_registry->GetHandle().emplace<Component>(m_handle, std::forward<Args>(args)...);
        }

        template <typename Component, typename... Args>
        auto PatchComponent(Args&&... args) -> void
        {
            m_registry->GetHandle().patch<Component>(m_handle, std::forward<Args>(args)...);
        }

        template <typename Component>
        auto RemoveComponent() const -> void
        {
            m_registry->GetHandle().remove<Component>(m_handle);
        }

        template <typename Component>
        [[nodiscard]] auto GetComponent() const -> Component&
        {
            return m_registry->GetHandle().get<Component>(m_handle);
        }

        template <typename Component>
        [[nodiscard]] auto TryGetComponent() const -> ObserverPointer<Component>
        {
            return m_registry->GetHandle().try_get<Component>(m_handle);
        }

        template <typename Component>
        [[nodiscard]] auto HasComponent() const -> bool
        {
            return m_registry->GetHandle().all_of<Component>(m_handle);
        }

        template <typename... Components>
        [[nodiscard]] auto HasAllComponents() const -> bool
        {
            return m_registry->GetHandle().all_of<Components...>(m_handle);
        }

        template <typename... Components>
        [[nodiscard]] auto HasAnyComponents() const -> bool
        {
            return m_registry->GetHandle().any_of<Components...>(m_handle);
        }

        template <typename... Components>
        [[nodiscard]] auto HasNoComponents() const -> bool
        {
            return !m_registry->GetHandle().any_of<Components...>(m_handle);
        }

        auto Destroy() noexcept -> void;

        [[nodiscard]] auto IsOrphan() const -> bool;

        [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_registry->GetHandle().valid(m_handle); }

        [[nodiscard]] inline auto IsNull() const noexcept -> bool { return m_handle == NullEntityHandle; }
        [[nodiscard]] inline auto GetHandle() const noexcept -> EntityHandle { return m_handle; }

        [[nodiscard]] auto operator ==(const Entity&) const -> bool = default;
        [[nodiscard]] auto operator !=(const Entity&) const -> bool = default;

        [[nodiscard]] inline operator bool() const noexcept { return m_handle != NullEntityHandle; }
        [[nodiscard]] inline operator u32() const noexcept { return static_cast<u32>(m_handle); }
    };
}

#endif
