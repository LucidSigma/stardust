#pragma once
#ifndef STARDUST_SCENE_H
#define STARDUST_SCENE_H

#include "stardust/utility/interfaces/INoncopyable.h"
#include "stardust/utility/interfaces/INonmovable.h"

#include "stardust/application/events/Events.h"
#include "stardust/ecs/entity/Entity.h"
#include "stardust/ecs/entity/EntityHandle.h"
#include "stardust/ecs/registry/EntityRegistry.h"
#include "stardust/graphics/renderer/Renderer.h"
#include "stardust/input/controller/GameController.h"
#include "stardust/input/controller/GameControllerCodes.h"
#include "stardust/input/keyboard/KeyCodes.h"
#include "stardust/input/mouse/MouseCodes.h"
#include "stardust/input/InputController.h"
#include "stardust/input/InputManager.h"
#include "stardust/scene/layer/LayerStack.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"
#include "stardust/window/Window.h"

namespace stardust
{
    namespace audio
    {
        class SoundSystem;
        class VolumeManager;
    }

    class Scene
        : private INoncopyable, private INonmovable
    {
    private:
        const String m_name;
        bool m_isFinished = false;

    protected:
        class Application& m_application;
        LayerStack m_layerStack;

    public:
        Scene(class Application& application, const StringView name);
        virtual ~Scene() noexcept = default;

        [[nodiscard]] inline virtual auto OnLoad() -> Status { return Status::Success; }
        inline virtual auto OnUnload() noexcept -> void { }

        inline virtual auto FixedUpdate(const f32 fixedDeltaTime) -> void { }
        inline virtual auto ProcessInput(const InputController& inputController, const InputManager& inputManager) -> void { }
        inline virtual auto PreUpdate(const f32 deltaTime) -> void { }
        inline virtual auto Update(const f32 deltaTime) -> void { };
        inline virtual auto PostUpdate(const f32 deltaTime) -> void { }

        virtual auto Render(graphics::Renderer& renderer) -> void = 0;

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

        [[nodiscard]] inline auto GetName() const noexcept -> const String& { return m_name; }
        [[nodiscard]] inline auto IsFinished() const noexcept -> bool { return m_isFinished; }
        inline auto Finish() noexcept -> void { m_isFinished = true; }

        [[nodiscard]] auto CreateEntity(const ObserverPointer<class EntityBundle> entityBundle = nullptr) -> Entity;
        [[nodiscard]] auto WrapEntityHandle(const EntityHandle entityHandle) -> Entity;

        [[nodiscard]] auto GetApplication() noexcept -> class Application&;
        [[nodiscard]] auto GetApplication() const noexcept -> const class Application&;

        [[nodiscard]] inline auto GetLayerStack() noexcept -> LayerStack& { return m_layerStack; }
        [[nodiscard]] inline auto GetLayerStack() const noexcept -> const LayerStack& { return m_layerStack; }

        [[nodiscard]] auto GetEntityRegistry() noexcept -> class EntityRegistry&;
        [[nodiscard]] auto GetEntityRegistry() const noexcept -> const class EntityRegistry&;

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto IterateEntities(const EntityRegistry::ExcludeComponents<Exclude...> excludedComponents = { }) -> EntityRegistry::IterableView<EntityRegistry::GetComponents<Components...>, EntityRegistry::ExcludeComponents<Exclude...>>
        {
            return GetEntityRegistry().IterateEntities<Components..., Exclude...>(excludedComponents);
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto IterateEntities(const EntityRegistry::ExcludeComponents<Exclude...> excludedComponents = { }) const -> EntityRegistry::IterableView<EntityRegistry::GetConstComponents<Components...>, EntityRegistry::ExcludeComponents<Exclude...>>
        {
            return GetEntityRegistry().IterateEntities<Components..., Exclude...>(excludedComponents);
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto GetEntity(const EntityRegistry::ExcludeComponents<Exclude...> excludedComponents = { }) -> Entity
        {
            const EntityHandle entityHandle = GetEntityRegistry().GetEntity<Components..., Exclude...>(excludedComponents);

            return WrapEntityHandle(entityHandle);
        }

        template <typename... Components, typename... Exclude>
        [[nodiscard]] auto TryGetEntity(const EntityRegistry::ExcludeComponents<Exclude...> excludedComponents = { }) -> Optional<Entity>
        {
            if (const EntityHandle entityHandle = GetEntityRegistry().GetEntity<Components..., Exclude...>(excludedComponents);
                entityHandle == NullEntityHandle)
            {
                return None;
            }
            else
            {
                return WrapEntityHandle(entityHandle);
            }
        }

        [[nodiscard]] auto GetWindow() noexcept -> Window&;
        [[nodiscard]] auto GetWindow() const noexcept -> const Window&;

        [[nodiscard]] auto GetUserPrefs() noexcept -> class UserPrefs&;
        [[nodiscard]] auto GetUserPrefs() const noexcept -> const class UserPrefs&;

        [[nodiscard]] auto GetControlPrefs() noexcept -> class ControlPrefs&;
        [[nodiscard]] auto GetControlPrefs() const noexcept -> const class ControlPrefs&;

        [[nodiscard]] auto GetLocale() noexcept -> class Locale&;
        [[nodiscard]] auto GetLocale() const noexcept -> const class Locale&;

        [[nodiscard]] auto GetSceneManager() noexcept -> class SceneManager&;
        [[nodiscard]] auto GetSceneManager() const noexcept -> const class SceneManager&;

        [[nodiscard]] auto GetRenderer() noexcept -> graphics::Renderer&;
        [[nodiscard]] auto GetRenderer() const noexcept -> const graphics::Renderer&;

        [[nodiscard]] auto GetCamera() noexcept -> class Camera2D&;
        [[nodiscard]] auto GetCamera() const noexcept -> const class Camera2D&;

        [[nodiscard]] auto GetSoundSystem() noexcept -> class audio::SoundSystem&;
        [[nodiscard]] auto GetSoundSystem() const noexcept -> const class audio::SoundSystem&;
        [[nodiscard]] auto GetVolumeManager() noexcept -> class audio::VolumeManager&;
        [[nodiscard]] auto GetVolumeManager() const noexcept -> const class audio::VolumeManager&;

        [[nodiscard]] auto GetInputController() const noexcept -> const InputController&;
        [[nodiscard]] auto GetKeyboardState() const noexcept -> const class KeyboardState&;
        [[nodiscard]] auto GetMouseState() const noexcept -> const class MouseState&;
        [[nodiscard]] auto GetGameControllerLobby() const noexcept -> const class GameControllerLobby&;
        [[nodiscard]] auto GetJoystickLobby() const noexcept -> const class JoystickLobby&;
        [[nodiscard]] auto GetInputManager() noexcept -> InputManager&;
        [[nodiscard]] auto GetInputManager() const noexcept -> const InputManager&;

        [[nodiscard]] auto GetGlobalSceneResources() noexcept -> class GlobalResources&;
        [[nodiscard]] auto GetGlobalSceneResources() const noexcept -> const class GlobalResources&;

        [[nodiscard]] auto GetScriptEngine() noexcept -> class ScriptEngine&;
        [[nodiscard]] auto GetScriptEngine() const noexcept -> const class ScriptEngine&;

        [[nodiscard]] auto GetTimestepController() noexcept -> class TimestepController&;
        [[nodiscard]] auto GetTimestepController() const noexcept -> const class TimestepController&;

        [[nodiscard]] auto GetElapsedTime() const noexcept -> f64;
    };
}

#endif
