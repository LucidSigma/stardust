#pragma once
#ifndef STARDUST_ENTITY_H
#define STARDUST_ENTITY_H

#include <utility>

#include <entt/entt.hpp>

#include "stardust/data/Pointers.h"
#include "stardust/scene/Scene.h"

namespace stardust
{
	class Entity
	{
	private:
		ObserverPtr<Scene> m_scene = nullptr;
		entt::entity m_handle = entt::null;

	public:
		Entity() = default;
		Entity(const entt::entity handle, Scene& scene);
		~Entity() noexcept = default;

		template <typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_scene->GetEntityRegistry().emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template <typename T>
		void RemoveComponent() const
		{
			return m_scene->GetEntityRegistry().remove<T>(m_handle);
		}

		template <typename T>
		T& GetComponent() const
		{
			return m_scene->GetEntityRegistry().get<T>(m_handle);
		}

		template <typename T>
		bool HasComponent() const
		{
			return m_scene->GetEntityRegistry().has<T>(m_handle);
		}

		entt::entity GetHandle() const noexcept { return m_handle; }

		bool operator ==(const Entity&) const = default;
		bool operator !=(const Entity&) const = default;

		inline operator bool() const noexcept { return m_handle != entt::null; }
		inline operator u32() const noexcept { return static_cast<u32>(m_handle); }
	};
}

#endif