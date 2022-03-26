#pragma once
#ifndef STARDUST_ENTITY_BUNDLE_H
#define STARDUST_ENTITY_BUNDLE_H

#include <utility>

#include "stardust/ecs/entity/Entity.h"
#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/scene/Scene.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class EntityBundle final
    {
    private:
        List<Entity> m_entities{ };
        ObserverPointer<Scene> m_scene = nullptr;

        bool m_willAutoDestroyEntities = false;

    public:
        EntityBundle() = default;
        EntityBundle(Scene& scene);
        ~EntityBundle() noexcept;

        auto Initialise(Scene& scene) -> void;

        auto CreateEntity() -> Entity;
        auto AddEntity(const Entity& entity) -> Entity;
        auto AddEntity(const EntityHandle entityHandle) -> Entity;

        template <typename Component, typename... Args>
        auto AddComponentToAllEntities(Args&&... args) -> void
        {
            for (auto& entity : m_entities)
            {
                entity.AddComponent<Component>(std::forward<Args>(args)...);
            }
        }

        template <typename Component, typename... Args>
        auto PatchComponentOfAllEntities(Args&&... args) -> void
        {
            for (auto& entity : m_entities)
            {
                entity.PatchComponent<Component>(std::forward<Args>(args)...);
            }
        }

        template <typename Component>
        auto RemoveComponentFromAllEntities() const -> void
        {
            for (auto& entity : m_entities)
            {
                entity.RemoveComponent<Component>();
            }
        }

        template <typename Component>
        [[nodiscard]] auto DoAllEntitiesHaveComponent() const -> bool
        {
            for (auto& entity : m_entities)
            {
                if (!entity.HasComponent<Component>())
                {
                    return false;
                }
            }

            return true;
        }

        template <typename... Components>
        [[nodiscard]] auto DoAllEntitiesHaveAllComponents() const -> bool
        {
            for (auto& entity : m_entities)
            {
                if (!entity.HasAllComponents<Components...>())
                {
                    return false;
                }
            }

            return true;
        }

        template <typename... Components>
        [[nodiscard]] auto DoAllEntitiesHaveAnyComponent() const -> bool
        {
            for (auto& entity : m_entities)
            {
                if (!entity.HasAnyComponents<Components...>())
                {
                    return false;
                }
            }

            return true;
        }

        template <typename... Components>
        [[nodiscard]] auto DoAllEntitiesHaveNoComponents() const -> bool
        {
            for (auto& entity : m_entities)
            {
                if (!entity.HasNoComponents<Components...>())
                {
                    return false;
                }
            }

            return true;
        }

        [[nodiscard]] auto AreAllEntitiesValid() const -> bool;
        auto RemoveInvalidEntities() -> void;

        auto DestroyAllEntities() noexcept -> void;

        [[nodiscard]] inline auto WillAutoDestroyEntities() const noexcept -> bool { return m_willAutoDestroyEntities; }
        inline auto SetEntityAutoDestroy(const bool willAutoDestroyEntities) noexcept -> void { m_willAutoDestroyEntities = willAutoDestroyEntities; }

        [[nodiscard]] inline auto GetEntities() noexcept -> List<Entity>& { return m_entities; }
        [[nodiscard]] inline auto GetEntities() const noexcept -> const List<Entity>& { return m_entities; }

        [[nodiscard]] auto IterateEntities() -> Generator<Entity&>;
        [[nodiscard]] auto IterateEntities() const -> Generator<const Entity&>;

        [[nodiscard]] inline auto begin() noexcept -> List<Entity>::iterator { return std::begin(m_entities); }
        [[nodiscard]] inline auto end() noexcept -> List<Entity>::iterator { return std::end(m_entities); }
        [[nodiscard]] inline auto cbegin() const noexcept -> List<Entity>::const_iterator { return std::cbegin(m_entities); }
        [[nodiscard]] inline auto cend() const noexcept -> List<Entity>::const_iterator { return std::cend(m_entities); }
    };
}

namespace std
{
    template <typename T>
    [[nodiscard]] inline auto begin(stardust::EntityBundle& entityBundle) noexcept -> decltype(auto) { return entityBundle.begin(); }

    template <typename T>
    [[nodiscard]] inline auto end(stardust::EntityBundle& entityBundle) noexcept -> decltype(auto) { return entityBundle.end(); }

    template <typename T>
    [[nodiscard]] inline auto cbegin(stardust::EntityBundle& entityBundle) noexcept -> decltype(auto) { return entityBundle.cbegin(); }

    template <typename T>
    [[nodiscard]] inline auto cend(stardust::EntityBundle& entityBundle) noexcept -> decltype(auto) { return entityBundle.cend(); }
}

#endif
