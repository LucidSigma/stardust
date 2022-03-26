#pragma once
#ifndef STARDUST_QUAD_BATCH_STATE_H
#define STARDUST_QUAD_BATCH_STATE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include "stardust/ecs/components/SpriteComponent.h"
#include "stardust/geometry/Shapes.h"
#include "stardust/graphics/pipeline/Pipeline.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/graphics/renderer/objects/Vertices.h"
#include "stardust/graphics/texture/Texture.h"
#include "stardust/types/Containers.h"
#include "stardust/types/MathTypes.h"
#include "stardust/types/Pointers.h"
#include "stardust/types/Primitives.h"

namespace stardust
{
    namespace graphics
    {
        class QuadBatchState final
            : private INoncopyable
        {
        public:
            struct CreateInfo final
            {
                usize maxShapesPerBatch;

                ObserverPointer<const Pipeline> batchPipeline;
                String textureArrayUniformName;

                ObserverPointer<const Texture> defaultTexture;
                usize maxTextureSlots;
            };

        private:
            static constexpr f32 s_DefaultTextureIndex = 0.0f;

            usize m_verticesPerBatch = 0u;
            usize m_indicesPerBatch = 0u;

            List<BatchQuadVertex> m_quadBuffer{ };
            BatchQuadVertex* m_bufferOffset = nullptr;

            u32 m_indexCount = 0u;

            List<ObserverPointer<const Texture>> m_textureSlots{ };
            usize m_maxTextureSlots = 0u;
            usize m_currentTextureSlotIndex = 1u;

            VertexLayout m_vertexLayout;
            VertexBuffer m_vertexBuffer;
            IndexBuffer m_indexBuffer;

        public:
            QuadBatchState() = default;
            explicit QuadBatchState(const CreateInfo& createInfo);

            QuadBatchState(QuadBatchState&& other) noexcept;
            auto operator =(QuadBatchState&& other) noexcept -> QuadBatchState&;

            ~QuadBatchState() noexcept;

            auto Initialise(const CreateInfo& createInfo) -> void;
            auto Destroy() noexcept -> void;

            [[nodiscard]] auto IsValid() const noexcept -> bool;

            auto Begin() -> void;
            auto Flush() -> void;

            auto BatchRectangle(const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
            auto BatchScreenRectangle(const UVector2 size, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
            auto BatchQuad(const geometry::Quad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;
            auto BatchScreenQuad(const geometry::ScreenQuad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void;

        private:
            auto InitialiseRenderObjects(const CreateInfo& createInfo) -> void;
            auto InitialiseTextureData(const CreateInfo& createInfo) -> void;

            auto RefreshIfRequired() -> void;

            [[nodiscard]] auto GetTextureIndex(const Texture& texture) -> usize;
        };
    }
}

#endif
