#pragma once
#ifndef STARDUST_ANIMATED_COMPONENT_H
#define STARDUST_ANIMATED_COMPONENT_H

#include "stardust/animation/Animation.h"
#include "stardust/data/Pointers.h"

namespace stardust
{
	namespace components
	{
		struct Animated
		{
			ObserverPtr<const Animation> animation;

			Animated()
				: animation(nullptr)
			{ }

			Animated(const Animation& animation)
				: animation(&animation)
			{ }

			Animated(ObserverPtr<const Animation> animation)
				: animation(animation)
			{ }

			~Animated() noexcept = default;
		};
	}
}

#endif