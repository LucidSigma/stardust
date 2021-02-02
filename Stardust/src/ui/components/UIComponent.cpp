#include "stardust/ui/components/UIComponent.h"

#include "stardust/ui/canvas/Canvas.h"

namespace stardust
{
	namespace ui
	{
		Component::Component(const Canvas& canvas, const Anchor anchor, const IVec2& anchorOffset)
			: m_owningCanvas(&canvas), m_anchor(anchor), m_anchorOffset(anchorOffset)
		{ }

		void Component::SetEnabled(const bool isEnabled)
		{
			m_isEnabled = isEnabled;

			if (m_isEnabled)
			{
				OnEnable();
			}
			else
			{
				OnDisable();
			}
		}
	}
}