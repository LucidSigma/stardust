#pragma once
#ifndef STARDUST_ENTITY_REGISTRY_H
#define STARDUST_ENTITY_REGISTRY_H

#include <entt/entt.hpp>

#include "stardust/scene/entity/Entity.h"
#include "stardust/scene/Scene.h"

namespace stardust
{
    class EntityRegistry
    {
    private:
        entt::registry m_handle;

    public:
        EntityRegistry() = default;
        ~EntityRegistry() noexcept = default;

        [[nodiscard]] Entity CreateEntity(Scene& scene);
        void DestroyEntity(const Entity& entity);

        void ClearAllEntities();

        [[nodiscard]] inline entt::registry& GetHandle() noexcept { return m_handle; }
        [[nodiscard]] inline const entt::registry& GetHandle() const noexcept { return m_handle; }
    };
}

#endif