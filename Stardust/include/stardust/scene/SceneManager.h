#pragma once
#ifndef STARDUST_SCENE_MANAGER_H
#define STARDUST_SCENE_MANAGER_H

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

#include "stardust/scene/Scene.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Pointers.h"

namespace stardust
{
    class SceneManager final
    {
    private:
        Queue<UniquePointer<Scene>> m_scenes{ };

    public:
        template <std::derived_from<Scene> S, typename... Args>
            requires std::is_constructible_v<S, Args...>
        auto PushScene(Args&&... args) -> void
        {
            m_scenes.emplace(std::make_unique<S>(std::forward<Args>(args)...));
        }

        auto PopScene() -> void;

        [[nodiscard]] inline auto CurrentScene() const -> const UniquePointer<Scene>& { return m_scenes.front(); }
        [[nodiscard]] inline auto IsEmpty() const noexcept -> bool { return m_scenes.empty(); }

        [[nodiscard]] inline auto GetSceneCount() const noexcept -> u32 { return static_cast<u32>(m_scenes.size()); }
    };
}

#endif
