#pragma once
#ifndef STARDUST_LINE_DRAW_STATE_H
#define STARDUST_LINE_DRAW_STATE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/geometry/Shapes.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/types/MathTypes.h"

namespace stardust
{
    namespace graphics
    {
        class LineDrawState final
            : private INoncopyable
        {
        private:
            VertexLayout m_vertexLayout;
            VertexBuffer m_vertexBuffer;

        public:
            LineDrawState() = default;

            LineDrawState(LineDrawState&& other) noexcept;
            auto operator =(LineDrawState&& other) noexcept -> LineDrawState&;

            ~LineDrawState() noexcept;

            auto Initialise() -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_vertexLayout.IsValid() && m_vertexBuffer.IsValid(); }

            auto DrawLine(const geometry::Line& line, const Matrix4& modelMatrix, const Colour& colour) -> void;
            auto DrawScreenLine(const geometry::ScreenLine& line, const Matrix4& modelMatrix, const Colour& colour) -> void;
        };
    }
}

#endif
