#pragma once
#ifndef STARDUST_UI_COMPONENT_H
#define STARDUST_UI_COMPONENT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/camera/Camera2D.h"
#include "stardust/ecs/bundle/EntityBundle.h"
#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/scene/Scene.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/ui/UI.h"

namespace stardust
{
    namespace ui
    {
        class Component final
            : private INoncopyable
        {
        public:
            using Key = String;

        protected:
            Key m_key;
            EntityHandle m_associatedEntityHandle = NullEntityHandle;

            UVector2 m_size = UVector2Zero;

            Anchor m_anchor = Anchor::TopLeft;
            IVector2 m_anchorOffset = IVector2Zero;

            Table m_styles;

            u32 m_depth;

            ObserverPointer<class Canvas> m_canvas = nullptr;
            ObserverPointer<Component> m_parent = nullptr;
            HashMap<Key, UniquePointer<Component>> m_childComponents{ };

            ObserverPointer<Scene> m_scene = nullptr;
            ObserverPointer<const Camera2D> m_camera = nullptr;

        public:
            friend class Canvas;

            Component(class Canvas& canvas, const StringView key, const EntityHandle associatedEntityHandle, const u32 depth, Scene& scene);

            Component(Component&&) noexcept = default;
            auto operator =(Component&&) noexcept -> Component& = default;

            ~Component() noexcept = default;

            [[nodiscard]] inline auto GetKey() const noexcept -> const String& { return m_key; }
            [[nodiscard]] inline auto GetAssociatedEntityHandle() const noexcept -> EntityHandle { return m_associatedEntityHandle; }

            [[nodiscard]] inline auto GetSize() const noexcept -> UVector2 { return m_size; }
            inline auto SetSize(const UVector2 size) noexcept -> void { m_size = size; }
            [[nodiscard]] inline auto GetWidth() const noexcept -> u32 { return m_size.x; }
            inline auto SetWidth(const u32 width) noexcept -> void { m_size.x = width; }
            [[nodiscard]] inline auto GetHeight() const noexcept -> u32 { return m_size.y; }
            inline auto SetHeight(const u32 height) noexcept -> void { m_size.y = height; }

            [[nodiscard]] inline auto GetAnchor() const noexcept -> Anchor { return m_anchor; }
            inline auto SetAnchor(const Anchor anchor) noexcept -> void { m_anchor = anchor; }
            [[nodiscard]] inline auto GetAnchorOffset() const noexcept -> IVector2 { return m_anchorOffset; }
            inline auto SetAnchorOffset(const IVector2 anchorOffset) noexcept -> void { m_anchorOffset = anchorOffset; }
            inline auto ShiftAnchorOffset(const IVector2 offset) noexcept -> void { m_anchorOffset += offset; }

            [[nodiscard]] inline auto GetStyles() const noexcept -> const Table& { return m_styles; }
            inline auto SetStyles(const Table& stylesTable) -> void { m_styles = stylesTable; }
            [[nodiscard]] inline auto HasStyle(const String& styleKey) const -> bool { return m_styles[styleKey] != Nil; }
            [[nodiscard]] inline auto GetStyle(const String& styleKey) const -> TableProxy { return m_styles[styleKey]; }

            template <typename T>
            [[nodiscard]] auto GetStyleOrDefault(const String& styleKey, const T& defaultValue) const -> T
            {
                return HasStyle(styleKey)
                    ? m_styles[styleKey]
                    : defaultValue;
            }

            template <typename T>
            inline auto SetStyle(const String& styleKey, const T& styleValue) -> void { m_styles[styleKey] = styleValue; }

            [[nodiscard]] auto GetAbsolutePosition() const -> IVector2;
            auto SetRelativePosition(const Anchor anchor, const IVector2 anchorOffset = IVector2Zero) noexcept -> void;

            [[nodiscard]] inline auto GetDepth() const noexcept -> u32 { return m_depth; }

            auto CreateChild(const Key& key, const ObserverPointer<EntityBundle> entityBundle = nullptr) -> Component&;
            auto CreateChildWithHook(const Key& key, const String& hookName, const ObserverPointer<EntityBundle> entityBundle = nullptr, const Optional<Any>& userData = None) -> Component&;
            auto AddChild(const Key& key, UniquePointer<Component>&& child) -> Component&;
            inline auto RemoveChild(const Key& key) -> void { m_childComponents.erase(key); }

            [[nodiscard]] inline auto GetChildren() noexcept -> HashMap<Key, UniquePointer<Component>>& { return m_childComponents; }
            [[nodiscard]] inline auto GetChildren() const noexcept -> const HashMap<Key, UniquePointer<Component>>& { return m_childComponents; }
            [[nodiscard]] inline auto GetChild(const Key& key) -> Component& { return *m_childComponents.at(key).get(); }
            [[nodiscard]] inline auto GetChild(const Key& key) const -> const Component& { return *m_childComponents.at(key).get(); }
            [[nodiscard]] auto GetChildRecursive(const Key& key) -> ObserverPointer<Component>;
            [[nodiscard]] auto GetChildRecursive(const Key& key) const -> ObserverPointer<const Component>;
            [[nodiscard]] inline auto GetChildCount() const noexcept -> u32 { return static_cast<u32>(m_childComponents.size()); }
            [[nodiscard]] inline auto HasChild(const Key& key) const -> bool { return m_childComponents.contains(key); }

            [[nodiscard]] inline auto GetOwningCanvas() noexcept -> Canvas& { return *m_canvas; }
            [[nodiscard]] inline auto GetOwningCanvas() const noexcept -> const Canvas& { return *m_canvas; }

            [[nodiscard]] inline auto HasParent() const noexcept -> bool { return m_parent != nullptr; }
            [[nodiscard]] inline auto GetParent() -> Component& { return *m_parent; }
            [[nodiscard]] inline auto GetParent() const -> const Component& { return *m_parent; }

        private:
            inline auto SetParent(const ObserverPointer<Component> parent) noexcept -> void { m_parent = parent; }
            inline auto SetDepth(const u32 depth) noexcept -> void { m_depth = depth; }
        };
    }
}

#endif
