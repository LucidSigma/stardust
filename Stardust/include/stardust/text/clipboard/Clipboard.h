#pragma once
#ifndef STARDUST_CLIPBOARD_H
#define STARDUST_CLIPBOARD_H

#include "stardust/types/Containers.h"

namespace stardust
{
    namespace clipboard
    {
        [[nodiscard]] extern auto HasText() -> bool;

        [[nodiscard]] extern auto GetText() -> String;
        extern auto SetText(const StringView text) -> void;
    }
}

#endif
