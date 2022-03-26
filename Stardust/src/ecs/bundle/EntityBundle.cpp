#include "stardust/ecs/bundle/EntityBundle.h"

#include <algorithm>
#include <vector>

namespace stardust
{
    EntityBundle::EntityBundle(Scene& scene)
    {
        Initialise(scene);
    }

    EntityBundle::~EntityBundle() noexcept
    {
        if (m_willAutoDestroyEntities)
        {
            DestroyAllEntities();
        }
    }

    auto EntityBundle::Initialise(Scene& scene) -> void
    {
        m_scene = &scene;
    }

    auto EntityBundle::CreateEntity() -> Entity
    {
        m_entities.push_back(m_scene->CreateEntity());

        return m_entities.back();
    }

    auto EntityBundle::AddEntity(const Entity& entity) -> Entity
    {
        m_entities.push_back(entity);

        return m_entities.back();
    }

    auto EntityBundle::AddEntity(const EntityHandle entityHandle) -> Entity
    {
        m_entities.push_back(Entity(entityHandle, *m_scene));

        return m_entities.back();
    }

    [[nodiscard]] auto EntityBundle::AreAllEntitiesValid() const -> bool
    {
        return std::ranges::all_of(
            m_entities,
            [](const Entity& entity) -> bool
            {
                return entity.IsValid();
            }
        );
    }

    auto EntityBundle::RemoveInvalidEntities() -> void
    {
        std::erase_if(
            m_entities,
            [](const Entity& entity) -> bool
            {
                return !entity.IsValid();
            }
        );
    }

    auto EntityBundle::DestroyAllEntities() noexcept -> void
    {
        for (auto& entity : m_entities)
        {
            entity.Destroy();
        }

        m_entities.clear();
    }
    
    [[nodiscard]] auto EntityBundle::IterateEntities() -> Generator<Entity&>
    {
        for (auto& entity : m_entities)
        {
            co_yield entity;
        }
    }
    
    [[nodiscard]] auto EntityBundle::IterateEntities() const -> Generator<const Entity&>
    {
        for (const auto& entity : m_entities)
        {
            co_yield entity;
        }
    }
}
