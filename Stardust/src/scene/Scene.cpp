#include "stardust/scene/Scene.h"

#include "stardust/application/Application.h"
#include "stardust/scene/entity/Entity.h"

namespace stardust
{
	Scene::Scene(Application& application, const String& name)
		: m_name(name), m_application(application), m_entityRegistry(application.GetEntityRegistry())
	{ }

	Entity Scene::CreateEntity()
	{
		return Entity(m_entityRegistry.create(), *this);
	}
}