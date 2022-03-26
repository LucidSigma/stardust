#include "stardust/ui/Canvas.h"

#include <iterator>
#include <memory>
#include <utility>

#include <pugixml/pugixml.hpp>

#include "stardust/ecs/components/UIComponentComponent.h"
#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"
#include "stardust/utility/string/String.h"

namespace stardust
{
    namespace ui
    {
        namespace
        {
            constexpr const char* CanvasNodeName = "canvas";
            constexpr const char* StyleTableAttributeName = "style-table";

            constexpr const char* KeyAttributeName = "key";
            constexpr const char* StylesAttributeName = "styles";
        }

        auto Canvas::RegisterComponentCreateHook(const String& componentName, const ComponentCreateHook& createHook) -> void
        {
            s_componentCreateHooks[componentName] = createHook;
        }

        Canvas::Canvas(const StringView xmlFilepath, Scene& scene, const ObserverPointer<EntityBundle> entityBundle, const Optional<Any>& userData)
        {
            Initialise(xmlFilepath, scene, entityBundle, userData);
        }

        auto Canvas::Initialise(const StringView xmlFilepath, Scene& scene, const ObserverPointer<EntityBundle> entityBundle, const Optional<Any>& userData) -> void
        {
            m_scene = &scene;
            auto xmlReadResult = vfs::ReadXML(xmlFilepath);

            if (xmlReadResult.is_err())
            {
                return;
            }

            const XML xml = std::move(xmlReadResult).unwrap();
            LoadComponentsFromXML(xml, xmlFilepath, entityBundle, userData);
        }

        auto Canvas::CreateChild(const Component::Key& key, const ObserverPointer<EntityBundle> entityBundle) -> Component&
        {
            Entity entity = m_scene->CreateEntity(entityBundle);

            m_childComponents[key] = std::make_unique<Component>(
                *this,
                key,
                entity.GetHandle(),
                1u,
                *m_scene
            );

            m_childComponents[key]->SetParent(nullptr);
            m_childComponents[key]->m_canvas = this;

            entity.AddComponent<components::UIComponent>(m_childComponents[key].get());

            return *m_childComponents[key];
        }

        auto Canvas::CreateChildWithHook(const Component::Key& key, const String& hookName, const ObserverPointer<EntityBundle> entityBundle, const Optional<Any>& userData) -> Component&
        {
            auto& childComponent = CreateChild(key, entityBundle);
            Entity childComponentEntity = m_scene->WrapEntityHandle(childComponent.GetAssociatedEntityHandle());

            if (s_componentCreateHooks.contains(hookName))
            {
                s_componentCreateHooks[hookName](
                    childComponent,
                    childComponentEntity,
                    *m_scene,
                    ComponentInfo{
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

        auto Canvas::AddChild(const Component::Key& key, UniquePointer<Component>&& child) -> Component&
        {
            m_childComponents[key] = std::move(child);
            m_childComponents[key]->SetParent(nullptr);
            m_childComponents[key]->m_canvas = this;
            m_childComponents[key]->SetDepth(1u);

            return *m_childComponents[key];
        }

        [[nodiscard]] auto Canvas::GetChildRecursive(const Component::Key& key) -> ObserverPointer<Component>
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

        [[nodiscard]] auto Canvas::GetChildRecursive(const Component::Key& key) const -> ObserverPointer<const Component>
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
        
        auto Canvas::LoadComponentsFromXML(const XML& xml, const StringView xmlFilepath, const ObserverPointer<EntityBundle> entityBundle, const Optional<Any>& userData) -> void
        {
            const pugi::xml_node canvasNode = xml.child(CanvasNodeName);

            if (canvasNode.empty())
            {
                Log::EngineWarn("No root node named canvas found in file {}.", xmlFilepath);

                return;
            }

            const pugi::xml_attribute styleTableAttribute = canvasNode.attribute(StyleTableAttributeName);

            if (styleTableAttribute.empty())
            {
                Log::EngineWarn("No style-table attribute found on canvas node in file {}.", xmlFilepath);

                return;
            }

            const Table globalStyleTable = m_scene->GetScriptEngine().Get<Table>(styleTableAttribute.value());

            struct ComponentElement final
            {
                pugi::xml_node component;
                ObserverPointer<Component> parent;

                u32 depth;
            };

            Stack<ComponentElement> components{ };

            for (const auto& component : canvasNode.children())
            {
                components.emplace(component, nullptr, 1u);
            }

            while (!components.empty())
            {
                const auto [component, parent, depth] = components.top();
                components.pop();

                if (!s_componentCreateHooks.contains(component.name()))
                {
                    Log::EngineWarn("Component \"{}\" in file {} has no registered create hook.", component.name(), xmlFilepath);

                    continue;
                }

                const pugi::xml_attribute keyAttribute = component.attribute(KeyAttributeName);

                if (keyAttribute.empty())
                {
                    Log::EngineWarn("Component \"{}\" in file {} has no key attribute.", component.name(), xmlFilepath);

                    continue;
                }

                const Component::Key componentKey = keyAttribute.value();
                const pugi::xml_attribute stylesAttribute = component.attribute(StylesAttributeName);

                if (stylesAttribute.empty())
                {
                    Log::EngineWarn("Component \"{}\" in file {} has no styles attribute.", component.name(), xmlFilepath);

                    continue;
                }

                Table stylesTable = globalStyleTable[stylesAttribute.value()];
                HashMap<String, String> otherAttributes{ };

                for (const auto& attribute : component.attributes())
                {
                    const String attributeName = attribute.name();

                    if (attributeName == KeyAttributeName || attributeName == StylesAttributeName)
                    {
                        continue;
                    }

                    otherAttributes[attributeName] = attribute.value();
                }

                const pugi::xml_node_iterator::difference_type childCount = std::distance(component.children().begin(), component.children().end());
                Optional<String> innerText = None;

                if (childCount == 1 && component.first_child().type() == pugi::node_pcdata)
                {
                    innerText = component.text().as_string();
                }

                Entity entity = m_scene->CreateEntity(entityBundle);
                UniquePointer<Component> createdComponent = std::make_unique<Component>(*this, componentKey, entity.GetHandle(), depth, *m_scene);
                createdComponent->SetStyles(stylesTable);

                const ObserverPointer<Component> createdComponentObserver = createdComponent.get();

                if (!(childCount == 1 && component.first_child().type() == pugi::node_pcdata))
                {
                    for (const auto& childComponent : component.children())
                    {
                        components.emplace(childComponent, createdComponent.get(), depth + 1u);
                    }
                }

                if (parent == nullptr)
                {
                    m_childComponents[componentKey] = std::move(createdComponent);
                }
                else
                {
                    parent->AddChild(componentKey, std::move(createdComponent));
                }

                entity.AddComponent<components::UIComponent>(createdComponentObserver);

                s_componentCreateHooks[component.name()](
                    *createdComponentObserver,
                    entity,
                    *m_scene,
                    ComponentInfo{
                        .name = component.name(),
                        .key = componentKey,
                        .attributes = std::move(otherAttributes),
                        .innerText = std::move(innerText),
                        .depth = depth,
                    },
                    userData
                );
            }

            m_didLoadSuccessfully = true;
        }
    }
}
