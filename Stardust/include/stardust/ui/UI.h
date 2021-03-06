#pragma once
#ifndef STARDUST_UI_H
#define STARDUST_UI_H

#include "stardust/data/Containers.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    class Application;

    namespace ui
    {
        [[nodiscard]] Status Initialise(const Application& application);
        void Shutdown() noexcept;

        [[nodiscard]] Status LoadFontFace(const StringView& filepath, const bool isFallbackFace = false);
    }
}

#endif