#include "stardust/system/System.h"

#include <SDL2/SDL.h>

namespace stardust
{
    namespace system
    {
        [[nodiscard]] u32 GetSystemRAMCount()
        {
            return static_cast<u32>(SDL_GetSystemRAM());
        }

        [[nodiscard]] u16 SwapEndianness(const u16 bits) noexcept
        {
            return SDL_Swap16(bits);
        }

        [[nodiscard]] u32 SwapEndianness(const u32 bits) noexcept
        {
            return SDL_Swap32(bits);
        }

        [[nodiscard]] u64 SwapEndianness(const u64 bits) noexcept
        {
            return SDL_Swap64(bits);
        }

        [[nodiscard]] f32 SwapEndianness(const f32 bits) noexcept
        {
            return SDL_SwapFloat(bits);
        }

        [[nodiscard]] u16 ConvertLittleToNativeEndianness(const u16 bits) noexcept
        {
            return SDL_SwapLE16(bits);
        }

        [[nodiscard]] u32 ConvertLittleToNativeEndianness(const u32 bits) noexcept
        {
            return SDL_SwapLE32(bits);
        }

        [[nodiscard]] u64 ConvertLittleToNativeEndianness(const u64 bits) noexcept
        {
            return SDL_SwapLE64(bits);
        }

        [[nodiscard]] f32 ConvertLittleToNativeEndianness(const f32 bits) noexcept
        {
            return SDL_SwapFloatLE(bits);
        }

        [[nodiscard]] u16 ConvertBigToNativeEndianness(const u16 bits) noexcept
        {
            return SDL_SwapBE16(bits);
        }

        [[nodiscard]] u32 ConvertBigToNativeEndianness(const u32 bits) noexcept
        {
            return SDL_SwapBE32(bits);
        }

        [[nodiscard]] u64 ConvertBigToNativeEndianness(const u64 bits) noexcept
        {
            return SDL_SwapBE64(bits);
        }

        [[nodiscard]] f32 ConvertBigToNativeEndianness(const f32 bits) noexcept
        {
            return SDL_SwapFloatBE(bits);
        }
    }
}