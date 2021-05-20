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

                gpuInfo.openGLVersionString = reinterpret_cast<const char*>(glGetString(GL_VERSION));

                if (!gpuInfo.openGLVersionString.empty())
                {
                    const String openGLVersion = string::Split(string::Trim(gpuInfo.openGLVersionString), ' ').front();
                    gpuInfo.openGLVersion = CreateVersionFromString(openGLVersion);
                }

                gpuInfo.glslVersionString = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

                if (!gpuInfo.glslVersionString.empty())
                {
                    const String glslVersion = string::Split(string::Trim(gpuInfo.glslVersionString), ' ').front();
                    gpuInfo.glslVersion = CreateVersionFromString(glslVersion);
                }

                GLint maxTextureSizeDimension = 0;
                glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSizeDimension);
                gpuInfo.maxTextureSize = static_cast<u64>(maxTextureSizeDimension) * static_cast<u64>(maxTextureSizeDimension) * 4ull;

                hasGPUInfoBeenQueried = true;
            }

            return gpuInfo;
        }
    }
}