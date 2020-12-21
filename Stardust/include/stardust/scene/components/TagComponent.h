#pragma once
#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include "stardust/data/Containers.h"

namespace stardust
{
	namespace components
	{
		struct Tag
		{
			String name;

			explicit Tag(const String& name = "")
				: name(name)
			{ }

			~Tag() noexcept = default;
		};
	}
}

#endif