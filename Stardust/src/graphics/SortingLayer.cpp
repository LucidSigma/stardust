#include "stardust/graphics/SortingLayer.h"

namespace stardust
{
	SortingLayer::SortingLayer(const StringView& name, const f32 z)
		: m_id(s_idCounter), m_name(name), m_z(z)
	{
		++s_idCounter;
	}
}