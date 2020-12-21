#include "stardust/scene/entity/Entity.h"

namespace stardust
{
	Entity::Entity(const entt::entity handle, Scene& scene)
		: m_handle(handle), m_scene(&scene)
	{ }
}