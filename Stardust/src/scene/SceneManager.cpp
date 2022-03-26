#include "stardust/scene/SceneManager.h"

namespace stardust
{
    auto SceneManager::PopScene() -> void
    {
        m_scenes.pop();
    }
}
