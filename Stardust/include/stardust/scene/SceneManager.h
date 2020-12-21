#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <memory>
#include <queue>
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

		template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Scene, T>&& std::is_constructible_v<T, Args...>>>
		void PushScene(Args&&... args)
		{
			m_scenes.push(eastl::make_unique<T>(std::forward<Args>(args)...));
		}

		void PopScene();

		inline const UniquePtr<Scene>& CurrentScene() const { return m_scenes.front(); }
		inline bool IsEmpty() const noexcept { return m_scenes.empty(); }

		inline usize GetSceneCount() const noexcept { return m_scenes.size(); }
	};
}

#endif