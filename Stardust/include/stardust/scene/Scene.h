#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	class Scene
		: private INoncopyable, private INonmovable
	{
	private:
		const String m_name;

	protected:
		class Application& m_application;
		entt::registry& m_entityRegistry;

	public:
		Scene(class Application& application, const String& name);
		virtual ~Scene() noexcept = default;

		[[nodiscard]] virtual Status OnLoad() = 0;
		virtual void OnUnload() noexcept = 0;

		inline virtual void FixedUpdate(const f32 fixedDeltaTime) { };
		virtual void ProcessInput() = 0;
		virtual void Update(const f32 deltaTime) = 0;
		inline virtual void LateUpdate(const f32 deltaTime) { }

		virtual void Render(const Renderer& renderer) const = 0;

		inline virtual void PollEvent(const SDL_Event& event) { }

		inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }
		inline const String& GetName() const noexcept { return m_name; }

	protected:
		class Entity CreateEntity();

		class Window& GetWindow() noexcept;
		const class Window& GetWindow() const noexcept;

		class Renderer& GetRenderer() noexcept;
		const class Renderer& GetRenderer() const noexcept;

		class Camera2D& GetCamera() noexcept;
		const class Camera2D& GetCamera() const noexcept;

		class SceneManager& GetSceneManager() noexcept;
		const class SceneManager& GetSceneManager() const noexcept;

		f32 GetElapsedTime() const noexcept;
	};
}

#endif