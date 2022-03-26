#pragma once
#ifndef STARDUST_LINE_BATCH_STATE_H
#define STARDUST_LINE_BATCH_STATE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/geometry/Shapes.h"
#include "stardust/graphics/colour/Colour.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/objects/Vertices.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        class LineBatchState final
            : private INoncopyable
        {
        private:
            usize m_verticesPerBatch = 0u;

            List<BatchLineVertex> m_lineBuffer{ };
            BatchLineVertex* m_bufferOffset = nullptr;

            u32 m_vertexCount = 0u;

            VertexLayout m_vertexLayout;
            VertexBuffer m_vertexBuffer;

        public:
            LineBatchState() = default;
            explicit LineBatchState(const usize maxShapesPerBatch);

            LineBatchState(LineBatchState&& other) noexcept;
            auto operator =(LineBatchState&& other) noexcept -> LineBatchState&;

            ~LineBatchState() noexcept;

            auto Initialise(const usize maxShapesPerBatch) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] inline auto IsValid() const noexcept -> bool { return m_vertexLayout.IsValid() && m_vertexBuffer.IsValid(); }

            auto Begin() -> void;
            auto Flush() -> void;

            auto BatchLine(const geometry::Line& line, const Matrix4& modelMatrix, const Colour& colour) -> void;
            auto BatchScreenLine(const geometry::ScreenLine& line, const Matrix4& modelMatrix, const Colour& colour) -> void;

        private:
            auto InitialiseRenderObjects() -> void;

            auto RefreshIfRequired() -> void;
        };
    }
}

#endif
