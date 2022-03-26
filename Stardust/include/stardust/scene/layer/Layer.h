#pragma once
#ifndef STARDUST_LAYER_H
#define STARDUST_LAYER_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/application/events/Events.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/input/InputController.h"
#include "stardust/input/InputManager.h"
#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    class Layer
        : private INoncopyable, private INonmovable
    {
    private:
        class Scene& m_owningScene;

        String m_name;

    public:
        Layer(class Scene& scene, const StringView name);
        virtual ~Layer() noexcept = default;

        [[nodiscard]] inline virtual auto OnAttach() -> Status { return Status::Success; }
        [[nodiscard]] inline virtual auto OnPostAttach() -> Status { return Status::Success; }
        inline virtual auto OnDetach() noexcept -> void { }

        inline virtual auto FixedUpdate(const f32 fixedDeltaTime) -> void { }
        inline virtual auto ProcessInput(const InputController& inputController, const InputManager& inputManager) -> void { }
        inline virtual auto PreUpdate(const f32 deltaTime) -> void { }
        inline virtual auto Update(const f32 deltaTime) -> void { }
        inline virtual auto PostUpdate(const f32 deltaTime) -> void { }

        inline virtual auto Render(graphics::Renderer& renderer) -> void { }

        inline virtual auto OnKeyDown(const events::KeyDown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnKeyUp(const events::KeyUp& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnTextInput(const events::TextInput& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnMouseButtonDown(const events::MouseButtonDown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnMouseButtonUp(const events::MouseButtonUp& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnMouseMotion(const events::MouseMotion& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnMouseScroll(const events::MouseScroll& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnGameControllerAdded(const events::GameControllerAdded& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerRemoved(const events::GameControllerRemoved& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerButtonDown(const events::GameControllerButtonDown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerButtonUp(const events::GameControllerButtonUp& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerAxisMotion(const events::GameControllerAxisMotion& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerTouchpadFingerDown(const events::GameControllerTouchpadFingerDown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerTouchpadFingerUp(const events::GameControllerTouchpadFingerUp& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerTouchpadFingerMotion(const events::GameControllerTouchpadFingerMotion& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerAccelerometerUpdate(const events::GameControllerAccelerometerUpdate& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnGameControllerGyroscopeUpdate(const events::GameControllerGyroscopeUpdate& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnJoystickAdded(const events::JoystickAdded& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnJoystickRemoved(const events::JoystickRemoved& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnJoystickButtonDown(const events::JoystickButtonDown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnJoystickButtonUp(const events::JoystickButtonUp& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnJoystickAxisMotion(const events::JoystickAxisMotion& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnJoystickHatSwitchMotion(const events::JoystickHatSwitchMotion& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnJoystickTrackballMotion(const events::JoystickTrackballMotion& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnTouchFingerDown(const events::TouchFingerDown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnTouchFingerUp(const events::TouchFingerUp& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnTouchFingerMotion(const events::TouchFingerMotion& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnMultiTouchFingerGesture(const events::MultiTouchFingerGesture& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnDollarGesturePerformed(const events::DollarGesturePerformed& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnDollarGestureRecorded(const events::DollarGestureRecorded& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnWindowShown(const events::WindowShown& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowHidden(const events::WindowHidden& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowMoved(const events::WindowMoved& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowResized(const events::WindowResized& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowMinimised(const events::WindowMinimised& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowMaximised(const events::WindowMaximised& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowRestored(const events::WindowRestored& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowMouseEnter(const events::WindowMouseEnter& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowMouseExit(const events::WindowMouseExit& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowFocusGained(const events::WindowFocusGained& event) -> EventStatus { return EventStatus::NotHandled; }
        inline virtual auto OnWindowFocusLost(const events::WindowFocusLost& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnLocaleChanged(const events::LocaleChanged& event) -> EventStatus { return EventStatus::NotHandled; }

        inline virtual auto OnUserEvent(const events::UserEvent& event) -> EventStatus { return EventStatus::NotHandled; }

        [[nodiscard]] auto GetOwningScene() noexcept -> class Scene&;
        [[nodiscard]] auto GetOwningScene() const noexcept -> const class Scene&;

        [[nodiscard]] auto GetApplication() noexcept -> class Application&;
        [[nodiscard]] auto GetApplication() const noexcept -> const class Application&;

        [[nodiscard]] auto GetEntityRegistry() noexcept -> class EntityRegistry&;
        [[nodiscard]] auto GetEntityRegistry() const noexcept -> const class EntityRegistry&;

        [[nodiscard]] auto GetWindow() noexcept -> class Window&;
        [[nodiscard]] auto GetWindow() const noexcept -> const class Window&;

        [[nodiscard]] auto GetLocale() noexcept -> class Locale&;
        [[nodiscard]] auto GetLocale() const noexcept -> const class Locale&;

        [[nodiscard]] auto GetRenderer() noexcept -> graphics::Renderer&;
        [[nodiscard]] auto GetRenderer() const noexcept -> const graphics::Renderer&;

        [[nodiscard]] auto GetCamera() noexcept -> class Camera2D&;
        [[nodiscard]] auto GetCamera() const noexcept -> const class Camera2D&;

        [[nodiscard]] auto GetGlobalSceneResources() noexcept -> class GlobalResources&;
        [[nodiscard]] auto GetGlobalSceneResources() const noexcept -> const class GlobalResources&;

        [[nodiscard]] auto GetScriptEngine() noexcept -> class ScriptEngine&;
        [[nodiscard]] auto GetScriptEngine() const noexcept -> const class ScriptEngine&;

        [[nodiscard]] auto GetTimestepController() noexcept -> class TimestepController&;
        [[nodiscard]] auto GetTimestepController() const noexcept -> const class TimestepController&;

        [[nodiscard]] auto GetElapsedTime() const noexcept -> f64;

        [[nodiscard]] inline auto GetName() const noexcept -> const String& { return m_name; }
    };
}

#endif
