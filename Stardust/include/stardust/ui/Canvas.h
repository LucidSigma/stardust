#pragma once
#ifndef STARDUST_CANVAS_H
#define STARDUST_CANVAS_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <functional>

#include "stardust/ecs/bundle/EntityBundle.h"
#include "stardust/ecs/entity/Entity.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/scene/Scene.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/ui/UIComponent.h"

namespace stardust
{
    namespace ui
    {
        class Canvas final
            : private INonmovable
        {
        public:
            struct ComponentInfo final
            {
                String name;
                Component::Key key;

                HashMap<String, String> attributes;
                Optional<String> innerText;

                u32 depth;
            };

            using ComponentCreateHook = std::function<auto(Component&, Entity&, Scene&, const ComponentInfo&, const Optional<Any>&) -> void>;

        private:
            inline static HashMap<String, ComponentCreateHook> s_componentCreateHooks{ };

            HashMap<Component::Key, UniquePointer<Component>> m_childComponents{ };

            ObserverPointer<Scene> m_scene = nullptr;
            bool m_didLoadSuccessfully = false;

        public:
            static auto RegisterComponentCreateHook(const String& componentName, const ComponentCreateHook& createHook) -> void;
            [[nodiscard]] inline static auto GetComponentCreateHooks() noexcept -> const HashMap<String, ComponentCreateHook>& { return s_componentCreateHooks; }

            Canvas() = default;
            Canvas(const StringView xmlFilepath, Scene& scene, const ObserverPointer<EntityBundle> entityBundle = nullptr, const Optional<Any>& userData = None);
            ~Canvas() noexcept = default;

            auto Initialise(const StringView xmlFilepath, Scene& scene, const ObserverPointer<EntityBundle> entityBundle = nullptr, const Optional<Any>& userData = None) -> void;

            auto CreateChild(const Component::Key& key, const ObserverPointer<EntityBundle> entityBundle = nullptr) -> Component&;
            auto CreateChildWithHook(const Component::Key& key, const String& hookName, const ObserverPointer<EntityBundle> entityBundle = nullptr, const Optional<Any>& userData = None) -> Component&;
            auto AddChild(const Component::Key& key, UniquePointer<Component>&& child) -> Component&;
            inline auto RemoveChild(const Component::Key& key) -> void { m_childComponents.erase(key); }

            [[nodiscard]] inline auto GetChildren() noexcept -> HashMap<Component::Key, UniquePointer<Component>>& { return m_childComponents; }
            [[nodiscard]] inline auto GetChildren() const noexcept -> const HashMap<Component::Key, UniquePointer<Component>>& { return m_childComponents; }
            [[nodiscard]] inline auto GetChild(const Component::Key& key) -> Component& { return *m_childComponents.at(key).get(); }
            [[nodiscard]] inline auto GetChild(const Component::Key& key) const -> const Component& { return *m_childComponents.at(key).get(); }
            [[nodiscard]] auto GetChildRecursive(const Component::Key& key) -> ObserverPointer<Component>;
            [[nodiscard]] auto GetChildRecursive(const Component::Key& key) const -> ObserverPointer<const Component>;
            [[nodiscard]] inline auto GetChildCount() const noexcept -> u32 { return static_cast<u32>(m_childComponents.size()); }
            [[nodiscard]] inline auto HasChild(const Component::Key& key) const -> bool { return m_childComponents.contains(key); }

            [[nodiscard]] inline auto DidLoadSuccessfully() const noexcept -> bool { return m_didLoadSuccessfully; }

        private:
            auto LoadComponentsFromXML(const XML& xml, const StringView xmlFilepath, const ObserverPointer<EntityBundle> entityBundle, const Optional<Any>& userData) -> void;
        };
    }
}

#endif
