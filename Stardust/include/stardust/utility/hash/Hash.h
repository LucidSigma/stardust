#pragma once
#ifndef STARDUST_HASH_H
#define STARDUST_HASH_H

#include "stardust/data/Containers.h"

namespace stardust
{
    namespace utility
    {
        [[nodiscard]] extern String GetMD5Hash(const StringView& text);
        [[nodiscard]] extern String GetSHA1Hash(const StringView& text);
        [[nodiscard]] extern String GetSHA256Hash(const StringView& text);
    }
}

#endif