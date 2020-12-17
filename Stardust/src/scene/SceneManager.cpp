#include "stardust/scene/SceneManager.h"

namespace stardust
{
	void SceneManager::PopScene()
	{
		m_scenes.pop();
	}
}