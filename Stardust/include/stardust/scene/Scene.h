#pragma once
#ifndef STARDUST_SCENE_H
#define STARDUST_SCENE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <entt/entt.hpp>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/keyboard/Keyboard.h"
#include "stardust/input/mouse/Mouse.h"
#include "stardust/input/Input.h"
#include "stardust/utility/status/Status.h"

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

		virtual void Render(Renderer& renderer) = 0;

		inline virtual void PollEvent(const SDL_Event& event) { }
		inline virtual void OnGameControllerAdded(GameController& gameController) { }
		inline virtual void OnGameControllerRemoved(const GameController& gameController) { }

		inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }
		inline const String& GetName() const noexcept { return m_name; }

		class Window& GetWindow() noexcept;
		const class Window& GetWindow() const noexcept;

		Renderer& GetRenderer() noexcept;
		const Renderer& GetRenderer() const noexcept;

		class Camera2D& GetCamera() noexcept;
		const class Camera2D& GetCamera() const noexcept;

		class InputManager& GetInputManager() noexcept;
		const class InputManager& GetInputManager() const noexcept;

		class SceneManager& GetSceneManager() noexcept;
		const class SceneManager& GetSceneManager() const noexcept;

		class ScriptEngine& GetScriptEngine() noexcept;
		const class ScriptEngine& GetScriptEngine() const noexcept;

		class SoundSystem& GetSoundSystem() noexcept;
		const class SoundSystem& GetSoundSystem() const noexcept;

		inline const Keyboard& GetKeyboardState() const noexcept { return Input::GetKeyboardState(); }
		inline const Mouse& GetMouseState() const noexcept { return Input::GetMouseState(); }

		f64 GetElapsedTime() const noexcept;

	protected:
		class Entity CreateEntity();
	};
}

#endif