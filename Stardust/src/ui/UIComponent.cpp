#include "stardust/ui/UIComponent.h"

#include <memory>
#include <utility>

#include <sol/sol.hpp>

#include "stardust/ecs/components/UIComponentComponent.h"
#include "stardust/ecs/entity/Entity.h"
#include "stardust/ui/Canvas.h"

namespace stardust
{
    namespace ui
    {
        Component::Component(Canvas& canvas, const StringView key, const EntityHandle associatedEntityHandle, const u32 depth, Scene& scene)
            : m_canvas(&canvas), m_key(key), m_associatedEntityHandle(associatedEntityHandle), m_styles(scene.GetScriptEngine().GetState(), sol::create), m_depth(depth), m_scene(&scene), m_camera(&scene.GetCamera())
        { }

        [[nodiscard]] auto Component::GetAbsolutePosition() const -> IVector2
        {
            if (m_parent == nullptr)
            {
                return GetVirtualScreenPositionFromAnchor(m_anchor, *m_camera, m_size, m_anchorOffset);
            }

            return m_parent->GetAbsolutePosition() + GetRelativePositionFromAnchor(m_anchor, m_parent->GetSize(), m_size, m_anchorOffset);
        }

        auto Component::SetRelativePosition(const Anchor anchor, const IVector2 anchorOffset) noexcept -> void
        {
            SetAnchor(anchor);
            SetAnchorOffset(anchorOffset);
        }

        auto Component::CreateChild(const Key& key, const ObserverPointer<EntityBundle> entityBundle) -> Component&
        {
            Entity entity = m_scene->CreateEntity(entityBundle);

            m_childComponents[key] = std::make_unique<Component>(
                *m_canvas,
                key,
                entity.GetHandle(),
                m_depth + 1u,
                *m_scene
            );
            m_childComponents[key]->SetParent(this);
            m_childComponents[key]->m_canvas = m_canvas;

            entity.AddComponent<components::UIComponent>(m_childComponents[key].get());

            return *m_childComponents[key];
        }

        auto Component::CreateChildWithHook(const Key& key, const String& hookName, const ObserverPointer<EntityBundle> entityBundle, const Optional<Any>& userData) -> Component&
        {
            Component& childComponent = CreateChild(key, entityBundle);
            Entity childComponentEntity = m_scene->WrapEntityHandle(childComponent.GetAssociatedEntityHandle());

            if (Canvas::GetComponentCreateHooks().contains(hookName))
            {
                Canvas::GetComponentCreateHooks().at(hookName)(
                    childComponent,
                    childComponentEntity,
                    *m_scene,
                    Canvas::ComponentInfo{
                        .name = hookName,
                        .key = key,
                        .attributes = { },
                        .innerText = "",
                        .depth = childComponent.GetDepth(),
                    },
                    userData
                );
            }

            return childComponent;
        }

        auto Component::AddChild(const Key& key, UniquePointer<Component>&& child) -> Component&
        {
            m_childComponents[key] = std::move(child);
            m_childComponents[key]->SetParent(this);
            m_childComponents[key]->m_canvas = m_canvas;
            m_childComponents[key]->SetDepth(m_depth + 1u);

            return *m_childComponents[key];
        }

        [[nodiscard]] auto Component::GetChildRecursive(const Key& key) -> ObserverPointer<Component>
        {
            if (HasChild(key))
            {
                return &GetChild(key);
            }

            for (const auto& [childKey, child] : m_childComponents)
            {
                const ObserverPointer<Component> foundChild = child->GetChildRecursive(key);

                if (foundChild != nullptr)
                {
                    return foundChild;
                }
            }

            return nullptr;
        }

        [[nodiscard]] auto Component::GetChildRecursive(const Key& key) const -> ObserverPointer<const Component>
        {
            if (HasChild(key))
            {
                return &GetChild(key);
            }

            for (const auto& [childKey, child] : m_childComponents)
            {
                const ObserverPointer<const Component> foundChild = child->GetChildRecursive(key);

                if (foundChild != nullptr)
                {
                    return foundChild;
                }
            }

            return nullptr;
        }
    }
}
