#pragma once
#ifndef STARDUST_UTILITY_H
#define STARDUST_UTILITY_H

#include "stardust/data/Containers.h"

namespace stardust
{
    namespace utility
    {
        template <typename... Types>
        struct Overload
            : Types...
        {
            using Types::operator ()...;
        };

        template <typename... Types>
        Overload(Types...) -> Overload<Types...>;

        [[nodiscard]] String ToBase64(const StringView& data);
        [[nodiscard]] String FromBase64(const StringView& base64);

        extern void OpenURL(const StringView& url);
    }

    namespace util = utility;
}

#endif