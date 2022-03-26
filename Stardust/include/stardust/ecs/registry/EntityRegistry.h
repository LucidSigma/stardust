#pragma once
#ifndef STARDUST_ENTITY_REGISTRY_H
#define STARDUST_ENTITY_REGISTRY_H

#include <functional>
#include <type_traits>

#include <entt/entt.hpp>

#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class Entity;

    class EntityRegistry final
    {
    public:
        template <typename... Components>
        using GetComponents = entt::get_t<Components...>;

        template <typename... Components>
        using GetConstComponents = entt::get_t<std::add_const_t<Components>...>;

        template <typename... Components>
        using ExcludeComponents = entt::exclude_t<Components...>;

        template <typename Components, typename Exclude>
        using View = entt::view<Components, Exclude>;

        template <typename Components, typename Exclude>
        using IterableView = entt::view<Components, Exclude>::iterable;

    private:
        entt::registry m_handle;

    public:
        [[nodiscard]] auto CreateEntity(class Scene& scene, const ObserverPointer<class EntityBundle> entityBundle = nullptr) -> Entity;
        auto DestroyEntity(const Entity& entity) -> void;

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto ViewEntities(const ExcludeComponents<Exclude...> excludedComponents = { }) -> View<GetComponents<Components...>, ExcludeComponents<Exclude...>>
        {
            return m_handle.view<Components..., Exclude...>(excludedComponents);
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto ViewEntities(const ExcludeComponents<Exclude...> excludedComponents = { }) const -> View<GetConstComponents<Components...>, ExcludeComponents<Exclude...>>
        {
            return m_handle.view<Components..., Exclude...>(excludedComponents);
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto IterateEntities(const ExcludeComponents<Exclude...> excludedComponents = { }) -> IterableView<GetComponents<Components...>, ExcludeComponents<Exclude...>>
        {
            return m_handle.view<Components..., Exclude...>(excludedComponents).each();
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto IterateEntities(const ExcludeComponents<Exclude...> excludedComponents = { }) const -> IterableView<GetConstComponents<Components...>, ExcludeComponents<Exclude...>>
        {
            return m_handle.view<Components..., Exclude...>(excludedComponents).each();
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto GetEntity(const ExcludeComponents<Exclude...> excludedComponents = { }) -> EntityHandle
        {
            for (const EntityHandle entityHandle : m_handle.view<Components..., Exclude...>(excludedComponents))
            {
                return entityHandle;
            }

            return NullEntityHandle;
        }

        template <typename Component>
        auto SortEntities(const std::function<auto(const Component&, const Component&) -> bool>& predicate) -> void
        {
            m_handle.sort<Component>(predicate);
        }

        template <typename To, typename From>
        auto SortEntitiesRespectTo() -> void
        {
            m_handle.sort<To, From>();
        }

        template <typename... Components>
        auto ClearComponents() -> void
        {
            m_handle.clear<Components...>();
        }

        auto ClearAllEntities() noexcept -> void;

        [[nodiscard]] inline auto GetHandle() noexcept -> entt::registry& { return m_handle; }
        [[nodiscard]] inline auto GetHandle() const noexcept -> const entt::registry& { return m_handle; }
    };
}

#endif
