#pragma once
#ifndef STARDUST_VSYNC_TYPE_H
#define STARDUST_VSYNC_TYPE_H

namespace stardust
{
	enum class VSyncType
	{
		None = 0,
		Regular = 1,
		Adaptive = -1,
	};
}

#endif