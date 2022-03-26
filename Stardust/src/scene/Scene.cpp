#include "stardust/scene/Scene.h"

#include "stardust/application/Application.h"
#include "stardust/audio/volume/VolumeManager.h"
#include "stardust/audio/SoundSystem.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/ecs/bundle/EntityBundle.h"
#include "stardust/input/controller/GameControllerLobby.h"
#include "stardust/input/joystick/JoystickLobby.h"
#include "stardust/input/keyboard/KeyboardState.h"
#include "stardust/input/mouse/MouseState.h"
#include "stardust/locale/Locale.h"
#include "stardust/preferences/ControlPrefs.h"
#include "stardust/preferences/UserPrefs.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/scene/resources/GlobalResources.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/time/timestep/TimestepController.h"

namespace stardust
{
    Scene::Scene(Application& application, const StringView name)
        : m_name(name), m_application(application)
    { }

    [[nodiscard]] auto Scene::CreateEntity(const ObserverPointer<EntityBundle> entityBundle) -> Entity
    {
        return GetEntityRegistry().CreateEntity(*this, entityBundle);
    }

    [[nodiscard]] auto Scene::WrapEntityHandle(const EntityHandle entityHandle) -> Entity
    {
        return Entity(entityHandle, *this);
    }

    [[nodiscard]] auto Scene::GetApplication() noexcept -> Application&
    {
        return m_application;
    }
    
    [[nodiscard]] auto Scene::GetApplication() const noexcept -> const Application&
    {
        return m_application;
    }

    [[nodiscard]] auto Scene::GetEntityRegistry() noexcept -> EntityRegistry&
    {
        return m_application.GetEntityRegistry();
    }

    [[nodiscard]] auto Scene::GetEntityRegistry() const noexcept -> const EntityRegistry&
    {
        return m_application.GetEntityRegistry();
    }

    [[nodiscard]] auto Scene::GetWindow() noexcept -> Window&
    {
        return m_application.GetWindow();
    }

    [[nodiscard]] auto Scene::GetWindow() const noexcept -> const Window&
    {
        return m_application.GetWindow();
    }

    [[nodiscard]] auto Scene::GetUserPrefs() noexcept -> UserPrefs&
    {
        return m_application.GetUserPrefs();
    }

    [[nodiscard]] auto Scene::GetUserPrefs() const noexcept -> const UserPrefs&
    {
        return m_application.GetUserPrefs();
    }

    [[nodiscard]] auto Scene::GetControlPrefs() noexcept -> ControlPrefs&
    {
        return m_application.GetControlPrefs();
    }

    [[nodiscard]] auto Scene::GetControlPrefs() const noexcept -> const ControlPrefs&
    {
        return m_application.GetControlPrefs();
    }

    [[nodiscard]] auto Scene::GetLocale() noexcept -> Locale&
    {
        return m_application.GetLocale();
    }

    [[nodiscard]] auto Scene::GetLocale() const noexcept -> const Locale&
    {
        return m_application.GetLocale();
    }

    [[nodiscard]] auto Scene::GetSceneManager() noexcept -> SceneManager&
    {
        return m_application.GetSceneManager();
    }

    [[nodiscard]] auto Scene::GetSceneManager() const noexcept -> const SceneManager&
    {
        return m_application.GetSceneManager();
    }

    [[nodiscard]] auto Scene::GetRenderer() noexcept -> graphics::Renderer&
    {
        return m_application.GetRenderer();
    }

    [[nodiscard]] auto Scene::GetRenderer() const noexcept -> const graphics::Renderer&
    {
        return m_application.GetRenderer();
    }

    [[nodiscard]] auto Scene::GetCamera() noexcept -> Camera2D&
    {
        return m_application.GetCamera();
    }

    [[nodiscard]] auto Scene::GetCamera() const noexcept -> const Camera2D&
    {
        return m_application.GetCamera();
    }

    [[nodiscard]] auto Scene::GetSoundSystem() noexcept -> audio::SoundSystem&
    {
        return m_application.GetSoundSystem();
    }

    [[nodiscard]] auto Scene::GetSoundSystem() const noexcept -> const audio::SoundSystem&
    {
        return m_application.GetSoundSystem();
    }

    [[nodiscard]] auto Scene::GetVolumeManager() noexcept -> audio::VolumeManager&
    {
        return m_application.GetVolumeManager();
    }

    [[nodiscard]] auto Scene::GetVolumeManager() const noexcept -> const audio::VolumeManager&
    {
        return m_application.GetVolumeManager();
    }

    [[nodiscard]] auto Scene::GetInputController() const noexcept -> const InputController&
    {
        return m_application.GetInputController();
    }

    [[nodiscard]] auto Scene::GetKeyboardState() const noexcept -> const KeyboardState&
    {
        return m_application.GetInputController().GetKeyboardState();
    }

    [[nodiscard]] auto Scene::GetMouseState() const noexcept -> const MouseState&
    {
        return m_application.GetInputController().GetMouseState();
    }

    [[nodiscard]] auto Scene::GetGameControllerLobby() const noexcept -> const GameControllerLobby&
    {
        return m_application.GetInputController().GetGameControllerLobby();
    }

    [[nodiscard]] auto Scene::GetJoystickLobby() const noexcept -> const JoystickLobby&
    {
        return m_application.GetInputController().GetJoystickLobby();
    }

    [[nodiscard]] auto Scene::GetInputManager() noexcept -> InputManager&
    {
        return m_application.GetInputManager();
    }

    [[nodiscard]] auto Scene::GetInputManager() const noexcept -> const InputManager&
    {
        return m_application.GetInputManager();
    }

    [[nodiscard]] auto Scene::GetGlobalSceneResources() noexcept -> GlobalResources&
    {
        return m_application.GetGlobalSceneResources();
    }

    [[nodiscard]] auto Scene::GetGlobalSceneResources() const noexcept -> const GlobalResources&
    {
        return m_application.GetGlobalSceneResources();
    }

    [[nodiscard]] auto Scene::GetScriptEngine() noexcept -> ScriptEngine&
    {
        return m_application.GetScriptEngine();
    }

    [[nodiscard]] auto Scene::GetScriptEngine() const noexcept -> const ScriptEngine&
    {
        return m_application.GetScriptEngine();
    }

    [[nodiscard]] auto Scene::GetTimestepController() noexcept -> TimestepController&
    {
        return m_application.GetTimestepController();
    }

    [[nodiscard]] auto Scene::GetTimestepController() const noexcept -> const TimestepController&
    {
        return m_application.GetTimestepController();
    }

    [[nodiscard]] auto Scene::GetElapsedTime() const noexcept -> f64
    {
        return m_application.GetElapsedTime();
    }
}
