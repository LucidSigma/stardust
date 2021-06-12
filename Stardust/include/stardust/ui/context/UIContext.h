#pragma once
#ifndef STARDUST_UI_CONTEXT_H
#define STARDUST_UI_CONTEXT_H

#include <RmlUi/Core.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"

namespace stardust
{
    namespace ui
    {
        class Context
        {
        private:
            Rml::Context* m_handle = nullptr;

        public:
            Context() = default;
            Context(const StringView& name, const UVec2& windowSize);

            // Move semantics here.

            ~Context() noexcept;

            void Initialise(const StringView& name, const UVec2& windowSize);
            void Destroy() noexcept;

            [[nodiscard]] inline bool IsValid() const noexcept { return m_handle != nullptr; }

            [[nodiscard]] Rml::Context* GetRawHandle() const noexcept { return m_handle; }
        };
    }
}

#endif