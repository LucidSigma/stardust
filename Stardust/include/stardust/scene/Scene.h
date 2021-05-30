#pragma once
#ifndef STARDUST_SCENE_H
#define STARDUST_SCENE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include <entt/entt.hpp>

#include "stardust/application/events/Events.h"
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
    namespace audio
    {
        class SoundSystem;
    }

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

        inline virtual void PollEvent(const Event& event) { }
        inline virtual void OnGameControllerAdded(GameController& gameController) { }
        inline virtual void OnGameControllerRemoved(const GameController& gameController) { }

        [[nodiscard]] inline const String& GetName() const noexcept { return m_name; }

        [[nodiscard]] class Application& GetApplication() noexcept;
        [[nodiscard]] const class Application& GetApplication() const noexcept;

        [[nodiscard]] inline entt::registry& GetEntityRegistry() noexcept { return m_entityRegistry; }
        [[nodiscard]] inline const entt::registry& GetEntityRegistry() const noexcept { return m_entityRegistry; }

        [[nodiscard]] class Window& GetWindow() noexcept;
        [[nodiscard]] const class Window& GetWindow() const noexcept;

        [[nodiscard]] Renderer& GetRenderer() noexcept;
        [[nodiscard]] const Renderer& GetRenderer() const noexcept;

        [[nodiscard]] class Camera2D& GetCamera() noexcept;
        [[nodiscard]] const class Camera2D& GetCamera() const noexcept;

        [[nodiscard]] class InputManager& GetInputManager() noexcept;
        [[nodiscard]] const class InputManager& GetInputManager() const noexcept;

        [[nodiscard]] class SceneManager& GetSceneManager() noexcept;
        [[nodiscard]] const class SceneManager& GetSceneManager() const noexcept;

        [[nodiscard]] class ScriptEngine& GetScriptEngine() noexcept;
        [[nodiscard]] const class ScriptEngine& GetScriptEngine() const noexcept;

        [[nodiscard]] audio::SoundSystem& GetSoundSystem() noexcept;
        [[nodiscard]] const audio::SoundSystem& GetSoundSystem() const noexcept;

        [[nodiscard]] inline const Keyboard& GetKeyboardState() const noexcept { return Input::GetKeyboardState(); }
        [[nodiscard]] inline const Mouse& GetMouseState() const noexcept { return Input::GetMouseState(); }

        [[nodiscard]] f64 GetElapsedTime() const noexcept;

    protected:
        class Entity CreateEntity();
    };
}

#endif