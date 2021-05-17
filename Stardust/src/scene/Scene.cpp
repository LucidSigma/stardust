#include "stardust/scene/Scene.h"

#include "stardust/application/Application.h"
#include "stardust/audio/SoundSystem.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/input/InputManager.h"
#include "stardust/scene/entity/Entity.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/window/Window.h"

namespace stardust
{
    Scene::Scene(Application& application, const String& name)
        : m_name(name), m_application(application), m_entityRegistry(application.GetEntityRegistry())
    { }

    [[nodiscard]] Application& Scene::GetApplication() noexcept
    {
        return m_application;
    }

    [[nodiscard]] const Application& Scene::GetApplication() const noexcept
    {
        return m_application;
    }

    [[nodiscard]] Window& Scene::GetWindow() noexcept
    {
        return m_application.GetWindow();
    }

    [[nodiscard]] const Window& Scene::GetWindow() const noexcept
    {
        return m_application.GetWindow();
    }

    [[nodiscard]] Renderer& Scene::GetRenderer() noexcept
    {
        return m_application.GetRenderer();
    }

    [[nodiscard]] const Renderer& Scene::GetRenderer() const noexcept
    {
        return m_application.GetRenderer();
    }

    [[nodiscard]] Camera2D& Scene::GetCamera() noexcept
    {
        return m_application.GetCamera();
    }

    [[nodiscard]] const Camera2D& Scene::GetCamera() const noexcept
    {
        return m_application.GetCamera();
    }

    [[nodiscard]] InputManager& Scene::GetInputManager() noexcept
    {
        return m_application.GetInputManager();
    }

    [[nodiscard]] const InputManager& Scene::GetInputManager() const noexcept
    {
        return m_application.GetInputManager();
    }

    [[nodiscard]] SceneManager& Scene::GetSceneManager() noexcept
    {
        return m_application.GetSceneManager();
    }

    [[nodiscard]] const SceneManager& Scene::GetSceneManager() const noexcept
    {
        return m_application.GetSceneManager();
    }

    [[nodiscard]] ScriptEngine& Scene::GetScriptEngine() noexcept
    {
        return m_application.GetScriptEngine();
    }

    [[nodiscard]] const ScriptEngine& Scene::GetScriptEngine() const noexcept
    {
        return m_application.GetScriptEngine();
    }

    [[nodiscard]] SoundSystem& Scene::GetSoundSystem() noexcept
    {
        return m_application.GetSoundSystem();
    }

    [[nodiscard]] const SoundSystem& Scene::GetSoundSystem() const noexcept
    {
        return m_application.GetSoundSystem();
    }

    [[nodiscard]] f64 Scene::GetElapsedTime() const noexcept
    {
        return m_application.GetElapsedTime();
    }

    Entity Scene::CreateEntity()
    {
        return Entity(m_entityRegistry.create(), *this);
    }
}