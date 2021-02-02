#include "stardust/ui/components/UIComponent.h"

#include "stardust/ui/canvas/Canvas.h"

namespace stardust
{
	namespace ui
	{
		Component::Component(const Canvas& canvas)
			: m_owningCanvas(&canvas)
		{ }
	}
}