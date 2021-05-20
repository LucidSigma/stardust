#pragma once
#ifndef STARDUST_GPU_H
#define STARDUST_GPU_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace system
    {
        struct GPUInfo
        {
            String name;
            String vendor;

            String openGLVersion;
            String glslVersion;

            u64 maxTextureSize;
        };

        [[nodiscard]] extern const GPUInfo& GetGPUInfo();
    }
}

#endif