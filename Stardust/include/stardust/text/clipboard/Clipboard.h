#pragma once
#ifndef STARDUST_CLIPBOARD_H
#define STARDUST_CLIPBOARD_H

#include "stardust/data/Containers.h"

namespace stardust
{
    namespace clipboard
    {
        [[nodiscard]] extern bool HasText();

        [[nodiscard]] extern String GetText();
        extern void SetText(const String& text);
    }
}

#endif CLIPBOARD_H