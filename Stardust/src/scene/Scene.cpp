#include "stardust/scene/Scene.h"

#include "stardust/application/Application.h"
#include "stardust/audio/SoundSystem.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/input/InputManager.h"
#include "stardust/scene/entity/Entity.h"
#include "stardust/scene/SceneManager.h"
#include "stardust/window/Window.h"

namespace stardust
{
	Scene::Scene(Application& application, const String& name)
		: m_name(name), m_application(application), m_entityRegistry(application.GetEntityRegistry())
	{ }

	Entity Scene::CreateEntity()
	{
		return Entity(m_entityRegistry.create(), *this);
	}

	Window& Scene::GetWindow() noexcept
	{
		return m_application.GetWindow();
	}

	const Window& Scene::GetWindow() const noexcept
	{
		return m_application.GetWindow();
	}

	Renderer& Scene::GetRenderer() noexcept
	{
		return m_application.GetRenderer();
	}

	const Renderer& Scene::GetRenderer() const noexcept
	{
		return m_application.GetRenderer();
	}

	Camera2D& Scene::GetCamera() noexcept
	{
		return m_application.GetCamera();
	}

	const Camera2D& Scene::GetCamera() const noexcept
	{
		return m_application.GetCamera();
	}

	InputManager& Scene::GetInputManager() noexcept
	{
		return m_application.GetInputManager();
	}

	const InputManager& Scene::GetInputManager() const noexcept
	{
		return m_application.GetInputManager();
	}

	SceneManager& Scene::GetSceneManager() noexcept
	{
		return m_application.GetSceneManager();
	}

	const SceneManager& Scene::GetSceneManager() const noexcept
	{
		return m_application.GetSceneManager();
	}

	SoundSystem& Scene::GetSoundSystem() noexcept
	{
		return m_application.GetSoundSystem();
	}

	const SoundSystem& Scene::GetSoundSystem() const noexcept
	{
		return m_application.GetSoundSystem();
	}

	f64 Scene::GetElapsedTime() const noexcept
	{
		return m_application.GetElapsedTime();
	}
}