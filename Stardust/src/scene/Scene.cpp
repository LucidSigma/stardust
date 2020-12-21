#include "stardust/scene/Scene.h"

#include "stardust/application/Application.h"

namespace stardust
{
	Scene::Scene(Application& application, const String& name)
		: m_name(name), m_application(application), m_entityRegistry(application.GetEntityRegistry())
	{ }
}