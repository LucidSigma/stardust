#pragma once
#ifndef STARDUST_SYSTEM_H
#define STARDUST_SYSTEM_H

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    namespace system
    {
        enum class Endianness
        {
            Little,
            Big,
        };

        enum class BuildType
        {
            Debug,
            Release,
        };

        [[nodiscard]] constexpr Endianness GetSystemEndianness() noexcept
        {
            if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                return Endianness::Little;
            }
            else
            {
                return Endianness::Big;
            }
        }

        [[nodiscard]] constexpr BuildType GetBuildType() noexcept
        {
        #ifdef NDEBUG
            return BuildType::Release;
        #else
            return BuildType::Debug;
        #endif
        }

        [[nodiscard]] extern u16 SwapEndianness(const u16 bits) noexcept;
        [[nodiscard]] extern u32 SwapEndianness(const u32 bits) noexcept;
        [[nodiscard]] extern u64 SwapEndianness(const u64 bits) noexcept;
        [[nodiscard]] extern f32 SwapEndianness(const f32 bits) noexcept;

        [[nodiscard]] extern u16 ConvertLittleToNativeEndianness(const u16 bits) noexcept;
        [[nodiscard]] extern u32 ConvertLittleToNativeEndianness(const u32 bits) noexcept;
        [[nodiscard]] extern u64 ConvertLittleToNativeEndianness(const u64 bits) noexcept;
        [[nodiscard]] extern f32 ConvertLittleToNativeEndianness(const f32 bits) noexcept;

        [[nodiscard]] extern u16 ConvertBigToNativeEndianness(const u16 bits) noexcept;
        [[nodiscard]] extern u32 ConvertBigToNativeEndianness(const u32 bits) noexcept;
        [[nodiscard]] extern u64 ConvertBigToNativeEndianness(const u64 bits) noexcept;
        [[nodiscard]] extern f32 ConvertBigToNativeEndianness(const f32 bits) noexcept;

        [[nodiscard]] extern u32 GetSystemRAMCount();

        [[nodiscard]] String GetPlatformName();
    }

    namespace sys = system;
}

#endif