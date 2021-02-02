#include "stardust/ui/components/UIComponent.h"

#include "stardust/ui/canvas/Canvas.h"

namespace stardust
{
	namespace ui
	{
		Component::Component(const Canvas& canvas)
			: m_owningCanvas(&canvas)
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