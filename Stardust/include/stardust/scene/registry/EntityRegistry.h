#pragma once
#ifndef STARDUST_ENTITY_REGISTRY_H
#define STARDUST_ENTITY_REGISTRY_H

#include <functional>

#include <entt/entt.hpp>

#include "stardust/data/Containers.h"

namespace stardust
{
    class EntityRegistry
    {
    private:
        entt::registry m_handle;

    public:
        EntityRegistry() = default;
        ~EntityRegistry() noexcept = default;

        [[nodiscard]] class Entity CreateEntity(class Scene& scene);
        void DestroyEntity(const class Entity& entity);

        template <typename... Args>
        [[nodiscard]] entt::view<Args...> ViewEntities()
        {
            return m_handle.view<Args...>();
        }

        template <typename... Args>
        [[nodiscard]] entt::view<Args...>::iterable_view IterateEntities()
        {
            return m_handle.view<Args...>().each();
        }

        template <typename T>
        void SortEntities(const std::function<bool(const T&, const T&)>& predicate)
        {
            m_handle.sort<T>(predicate);
        }

        [[nodiscard]] bool IsEntityValid(const class Entity& entity) const;

        template <typename... Args>
        void ClearComponents()
        {
            m_handle.clear<Args...>();
        }

        void ClearAllEntities();

        [[nodiscard]] inline entt::registry& GetHandle() noexcept { return m_handle; }
        [[nodiscard]] inline const entt::registry& GetHandle() const noexcept { return m_handle; }
    };
}

#endif