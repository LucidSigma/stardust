#include "stardust/ui/Canvas.h"

namespace stardust
{
	namespace ui
	{
		Canvas::Canvas(Renderer& renderer)
		{
			Initialise(renderer);
		}

		void Canvas::Initialise(Renderer & renderer)
		{
			m_renderer = &renderer;
		}
	}
}