#include "stardust/utility/Utility.h"

#include <SDL2/SDL.h>
#include <stb/stb_image_write.h>

namespace stardust
{
    namespace utility
    {
        auto OpenURL(const StringView url) -> void
        {
            SDL_OpenURL(url.data());
        }

        auto WriteToPNG(const StringView filepath, const graphics::PixelData& pixelData) -> Status
        {
            const i32 imageWriteResult = stbi_write_png(
                filepath.data(),
                static_cast<i32>(pixelData.extent.x),
                static_cast<i32>(pixelData.extent.y),
                static_cast<i32>(pixelData.channelCount),
                pixelData.pixels.data(),
                static_cast<i32>(pixelData.extent.x * pixelData.channelCount)
            );

            return imageWriteResult != 0 ? Status::Success : Status::Fail;
        }
    }
}
