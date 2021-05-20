#include "stardust/system/gpu/GPU.h"

#include <glad/glad.h>

namespace stardust
{
    namespace system
    {
        namespace
        {
            GPUInfo gpuInfo{ };
            bool hasGPUInfoBeenQueried = false;
        }

        [[nodiscard]] const GPUInfo& GetGPUInfo()
        {
            if (!hasGPUInfoBeenQueried)
            {
                gpuInfo.name = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
                gpuInfo.vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

                gpuInfo.openGLVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
                gpuInfo.glslVersion = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

                GLint maxTextureSizeDimension = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSizeDimension);
                gpuInfo.maxTextureSize = static_cast<u64>(maxTextureSizeDimension) * static_cast<u64>(maxTextureSizeDimension) * 4ull;

                hasGPUInfoBeenQueried = true;
            }

            return gpuInfo;
        }
    }
}