#include "stardust/scene/layer/Layer.h"

#include "stardust/application/Application.h"
#include "stardust/camera/Camera2D.h"
#include "stardust/ecs/registry/EntityRegistry.h"
#include "stardust/locale/Locale.h"
#include "stardust/scene/resources/GlobalResources.h"
#include "stardust/scene/Scene.h"
#include "stardust/scripting/ScriptEngine.h"
#include "stardust/time/timestep/TimestepController.h"
#include "stardust/window/Window.h"

namespace stardust
{
    Layer::Layer(Scene& scene, const StringView name)
        : m_owningScene(scene), m_name(name)
    { }

    [[nodiscard]] auto Layer::GetOwningScene() noexcept -> Scene&
    {
        return m_owningScene;
    }

    [[nodiscard]] auto Layer::GetOwningScene() const noexcept -> const Scene&
    {
        return m_owningScene;
    }

    [[nodiscard]] auto Layer::GetApplication() noexcept -> Application&
    {
        return GetOwningScene().GetApplication();
    }

    [[nodiscard]] auto Layer::GetApplication() const noexcept -> const Application&
    {
        return GetOwningScene().GetApplication();
    }

    [[nodiscard]] auto Layer::GetEntityRegistry() noexcept -> EntityRegistry&
    {
        return GetOwningScene().GetEntityRegistry();
    }

    [[nodiscard]] auto Layer::GetEntityRegistry() const noexcept -> const EntityRegistry&
    {
        return GetOwningScene().GetEntityRegistry();
    }

    [[nodiscard]] auto Layer::GetWindow() noexcept -> Window&
    {
        return GetOwningScene().GetWindow();
    }

    [[nodiscard]] auto Layer::GetWindow() const noexcept -> const Window&
    {
        return GetOwningScene().GetWindow();
    }

    [[nodiscard]] auto Layer::GetLocale() noexcept -> Locale&
    {
        return GetOwningScene().GetLocale();
    }

    [[nodiscard]] auto Layer::GetLocale() const noexcept -> const Locale&
    {
        return GetOwningScene().GetLocale();
    }

    [[nodiscard]] auto Layer::GetRenderer() noexcept -> graphics::Renderer&
    {
        return GetOwningScene().GetRenderer();
    }

    [[nodiscard]] auto Layer::GetRenderer() const noexcept -> const graphics::Renderer&
    {
        return GetOwningScene().GetRenderer();
    }

    [[nodiscard]] auto Layer::GetCamera() noexcept -> Camera2D&
    {
        return GetOwningScene().GetCamera();
    }

    [[nodiscard]] auto Layer::GetCamera() const noexcept -> const Camera2D&
    {
        return GetOwningScene().GetCamera();
    }

    [[nodiscard]] auto Layer::GetGlobalSceneResources() noexcept -> GlobalResources&
    {
        return GetOwningScene().GetGlobalSceneResources();
    }

    [[nodiscard]] auto Layer::GetGlobalSceneResources() const noexcept -> const GlobalResources&
    {
        return GetOwningScene().GetGlobalSceneResources();
    }

    [[nodiscard]] auto Layer::GetScriptEngine() noexcept -> ScriptEngine&
    {
        return GetOwningScene().GetScriptEngine();
    }

    [[nodiscard]] auto Layer::GetScriptEngine() const noexcept -> const ScriptEngine&
    {
        return GetOwningScene().GetScriptEngine();
    }

    [[nodiscard]] auto Layer::GetTimestepController() noexcept -> TimestepController&
    {
        return GetOwningScene().GetTimestepController();
    }

    [[nodiscard]] auto Layer::GetTimestepController() const noexcept -> const TimestepController&
    {
        return GetOwningScene().GetTimestepController();
    }

    [[nodiscard]] auto Layer::GetElapsedTime() const noexcept -> f64
    {
        return GetOwningScene().GetElapsedTime();
    }
}
