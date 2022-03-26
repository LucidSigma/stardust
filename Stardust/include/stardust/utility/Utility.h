#pragma once
#ifndef STARDUST_UTILITY_H
#define STARDUST_UTILITY_H

#include "stardust/graphics/Graphics.h"
#include "stardust/types/Containers.h"
#include "stardust/utility/error_handling/Status.h"

namespace stardust
{
    namespace utility
    {
        template <typename... Types>
        struct VariantOverloader final
            : public Types...
        {
            using Types::operator ()...;
        };

        template <typename... Types>
        VariantOverloader(Types...) -> VariantOverloader<Types...>;

        template <typename T>
        constexpr bool DependentTrue = true;

        template <typename T>
        constexpr bool DependentFalse = false;

        extern auto OpenURL(const StringView url) -> void;

        extern auto WriteToPNG(const StringView filepath, const graphics::PixelData& pixelData) -> Status;
    }

    namespace util = utility;
}

#endif
