#pragma once
#ifndef CPU_H
#define CPU_H

#include "stardust/data/Types.h"

namespace stardust
{
	namespace cpu
	{
		[[nodiscard]] extern u32 GetL1CacheLineSize();
		[[nodiscard]] extern u32 GetCPUCount();
		[[nodiscard]] extern u32 GetSystemRAMCount();
	}
}

#endif