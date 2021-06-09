#pragma once
#ifndef STARDUST_SCENE_MANAGER_H
#define STARDUST_SCENE_MANAGER_H

#include <concepts>
#include <type_traits>
#include <utility>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/scene/Scene.h"

namespace stardust
{
    class SceneManager
    {
    private:
        Queue<UniquePtr<Scene>> m_scenes{ };

    public:
        SceneManager() = default;
        ~SceneManager() noexcept = default;

        template <std::derived_from<Scene> T, typename... Args>
            requires std::is_constructible_v<T, Args...>
        void PushScene(Args&&... args)
        {
            m_scenes.push(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void PopScene();

        [[nodiscard]] inline const UniquePtr<Scene>& CurrentScene() const { return m_scenes.front(); }
        [[nodiscard]] inline bool IsEmpty() const noexcept { return m_scenes.empty(); }

        [[nodiscard]] inline usize GetSceneCount() const noexcept { return m_scenes.size(); }
    };
}

#endif