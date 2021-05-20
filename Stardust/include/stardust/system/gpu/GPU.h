#pragma once
#ifndef STARDUST_GPU_H
#define STARDUST_GPU_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/version/Version.h"

namespace stardust
{
    namespace system
    {
        struct GPUInfo
        {
            String name;
            String vendor;

            VersionInfo openGLVersion;
            String openGLVersionString;

            VersionInfo glslVersion;
            String glslVersionString;

            u64 maxTextureSize;
        };

        [[nodiscard]] extern const GPUInfo& GetGPUInfo();
    }
}

#endif