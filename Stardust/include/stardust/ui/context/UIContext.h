#pragma once
#ifndef STARDUST_UI_CONTEXT_H
#define STARDUST_UI_CONTEXT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <RmlUi/Core.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"

namespace stardust
{
    namespace ui
    {
        class Context
            : private INoncopyable
        {
        private:
            Rml::Context* m_handle = nullptr;

        public:
            Context() = default;
            Context(const StringView& name, const UVec2& dimensions);

            Context(Context&& other) noexcept;
            Context& operator =(Context&& other) noexcept;

            ~Context() noexcept;

            void Initialise(const StringView& name, const UVec2& dimensions);
            void Destroy() noexcept;

            [[nodiscard]] Rml::ElementDocument* LoadRMLDocument(const StringView& filepath) const;

            [[nodiscard]] inline bool IsValid() const noexcept { return m_handle != nullptr; }

            [[nodiscard]] Rml::Context* GetRawHandle() const noexcept { return m_handle; }
        };
    }
}

#endif