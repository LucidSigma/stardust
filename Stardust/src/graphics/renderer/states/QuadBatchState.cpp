#include "stardust/graphics/renderer/states/QuadBatchState.h"

#include <algorithm>
#include <numeric>
#include <utility>

#include <ANGLE/GLES3/gl3.h>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/graphics/renderer/objects/VertexAttribute.h"
#include "stardust/graphics/renderer/objects/VertexLayoutBuilder.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace graphics
    {
        QuadBatchState::QuadBatchState(const CreateInfo& createInfo)
        {
            Initialise(createInfo);
        }

        QuadBatchState::QuadBatchState(QuadBatchState&& other) noexcept
        {
            Destroy();

            m_verticesPerBatch = std::exchange(other.m_verticesPerBatch, 0u);
            m_indicesPerBatch = std::exchange(other.m_indicesPerBatch, 0u);

            m_quadBuffer = std::move(other.m_quadBuffer);
            m_bufferOffset = std::exchange(other.m_bufferOffset, nullptr);

            m_indexCount = std::exchange(other.m_indexCount, 0u);

            m_textureSlots = std::move(other.m_textureSlots);
            m_maxTextureSlots = std::exchange(other.m_maxTextureSlots, 0u);
            m_currentTextureSlotIndex = std::exchange(other.m_currentTextureSlotIndex, 1u);

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);
            m_indexBuffer = std::move(other.m_indexBuffer);
        }

        auto QuadBatchState::operator =(QuadBatchState&& other) noexcept -> QuadBatchState&
        {
            Destroy();

            m_quadBuffer = std::move(other.m_quadBuffer);
            m_bufferOffset = std::exchange(other.m_bufferOffset, nullptr);

            m_indexCount = std::exchange(other.m_indexCount, 0u);

            m_textureSlots = std::move(other.m_textureSlots);
            m_maxTextureSlots = std::exchange(other.m_maxTextureSlots, 0u);
            m_currentTextureSlotIndex = std::exchange(other.m_currentTextureSlotIndex, 1u);

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);
            m_indexBuffer = std::move(other.m_indexBuffer);

            return *this;
        }
        
        QuadBatchState::~QuadBatchState() noexcept
        {
            Destroy();
        }

        auto QuadBatchState::Initialise(const CreateInfo& createInfo) -> void
        {
            m_verticesPerBatch = createInfo.maxShapesPerBatch * 4u;
            m_indicesPerBatch = createInfo.maxShapesPerBatch * 6u;

            InitialiseRenderObjects(createInfo);

            m_quadBuffer.resize(m_verticesPerBatch);
            m_bufferOffset = m_quadBuffer.data();

            InitialiseTextureData(createInfo);
        }
        
        auto QuadBatchState::Destroy() noexcept -> void
        {
            m_vertexLayout.Destroy();
            m_vertexBuffer.Destroy();
            m_indexBuffer.Destroy();
        }
        
        [[nodiscard]] auto QuadBatchState::IsValid() const noexcept -> bool
        {
            return m_vertexLayout.IsValid() && m_vertexBuffer.IsValid() && m_indexBuffer.IsValid();
        }
        
        auto QuadBatchState::Begin() -> void
        {
            m_bufferOffset = m_quadBuffer.data();
            m_indexCount = 0u;
            m_currentTextureSlotIndex = 1u;
        }
        
        auto QuadBatchState::Flush() -> void
        {
            const isize batchSize = m_bufferOffset - m_quadBuffer.data();
            m_vertexBuffer.SetSubData(m_quadBuffer, static_cast<usize>(batchSize));

            for (usize i = 0u; i < m_currentTextureSlotIndex; ++i)
            {
                if (m_textureSlots[i] != nullptr) [[likely]]
                {
                    m_textureSlots[i]->Bind(static_cast<Texture::BindingIndex>(i));
                }
            }

            m_vertexLayout.Bind();
            m_vertexLayout.DrawIndexed(m_indexBuffer, m_indexCount);
            m_vertexLayout.Unbind();

            for (usize i = 0u; i < m_currentTextureSlotIndex; ++i)
            {
                if (m_textureSlots[i] != nullptr) [[likely]]
                {
                    m_textureSlots[i]->Unbind();
                }
            }
        }

        auto QuadBatchState::BatchRectangle(const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            RefreshIfRequired();

            const f32 textureIndex = sprite.texture != nullptr
                ? static_cast<f32>(GetTextureIndex(*sprite.texture))
                : s_DefaultTextureIndex;

            const TextureCoordinatePair textureCoordinates{
                .lowerLeft = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().lowerLeft
                    : Vector2Zero,
                .upperRight = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().upperRight
                    : Vector2One,
            };

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ 0.5f, 0.5f, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.upperRight;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ 0.5f, -0.5f, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.upperRight.x, textureCoordinates.lowerLeft.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ -0.5f, -0.5f, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.lowerLeft;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ -0.5f, 0.5f, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.lowerLeft.x, textureCoordinates.upperRight.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_indexCount += 6u;
        }
        
        auto QuadBatchState::BatchScreenRectangle(const UVector2 size, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            RefreshIfRequired();

            const f32 textureIndex = sprite.texture != nullptr
                ? static_cast<f32>(GetTextureIndex(*sprite.texture))
                : s_DefaultTextureIndex;

            const TextureCoordinatePair textureCoordinates{
                .lowerLeft = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().lowerLeft
                    : Vector2Zero,
                .upperRight = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().upperRight
                    : Vector2One,
            };

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ size.x, 0.0f, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.upperRight;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ size, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.upperRight.x, textureCoordinates.lowerLeft.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ 0.0f, size.y, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.lowerLeft;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ 0.0f, 0.0f, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.lowerLeft.x, textureCoordinates.upperRight.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_indexCount += 6u;
        }

        auto QuadBatchState::BatchQuad(const geometry::Quad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            RefreshIfRequired();

            const f32 textureIndex = sprite.texture != nullptr
                ? static_cast<f32>(GetTextureIndex(*sprite.texture))
                : s_DefaultTextureIndex;

            const TextureCoordinatePair textureCoordinates{
                .lowerLeft = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().lowerLeft
                    : Vector2Zero,
                .upperRight = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().upperRight
                    : Vector2One,
            };

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.upperRight, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.upperRight;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.lowerRight, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.upperRight.x, textureCoordinates.lowerLeft.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.lowerLeft, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.lowerLeft;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.upperLeft, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.lowerLeft.x, textureCoordinates.upperRight.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_indexCount += 6u;
        }

        auto QuadBatchState::BatchScreenQuad(const geometry::ScreenQuad& quad, const Matrix4& modelMatrix, const components::Sprite& sprite) -> void
        {
            RefreshIfRequired();

            const f32 textureIndex = sprite.texture != nullptr
                ? static_cast<f32>(GetTextureIndex(*sprite.texture))
                : s_DefaultTextureIndex;

            const TextureCoordinatePair textureCoordinates{
                .lowerLeft = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().lowerLeft
                    : Vector2Zero,
                .upperRight = sprite.subTextureArea.has_value()
                    ? sprite.subTextureArea.value().upperRight
                    : Vector2One,
            };

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.upperRight, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.upperRight;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.lowerRight, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.upperRight.x, textureCoordinates.lowerLeft.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.lowerLeft, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = textureCoordinates.lowerLeft;
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ quad.upperLeft, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(sprite.colourMod);
            m_bufferOffset->textureCoordinates = Vector2{ textureCoordinates.lowerLeft.x, textureCoordinates.upperRight.y };
            m_bufferOffset->textureIndex = textureIndex;
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_indexCount += 6u;
        }

        auto QuadBatchState::InitialiseRenderObjects(const CreateInfo& createInfo) -> void
        {
            m_vertexBuffer.Initialise(m_verticesPerBatch * sizeof(BatchQuadVertex), BufferUsage::Dynamic);

            m_vertexLayout = VertexLayoutBuilder()
                .AddAttribute(VertexAttribute{
                    .elementCount = 2u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddAttribute(VertexAttribute{
                    .elementCount = 4u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddAttribute(VertexAttribute{
                    .elementCount = 2u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddAttribute(VertexAttribute{
                    .elementCount = 1u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddAttribute(VertexAttribute{
                    .elementCount = 1u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddVertexBuffer(m_vertexBuffer)
                .Build();

            List<u32> indices(m_indicesPerBatch);
            u32 indexOffset = 0u;

            for (usize i = 0u; i < m_indicesPerBatch; i += 6u)
            {
                indices[i + 0u] = 0u + indexOffset;
                indices[i + 1u] = 1u + indexOffset;
                indices[i + 2u] = 3u + indexOffset;

                indices[i + 3u] = 1u + indexOffset;
                indices[i + 4u] = 2u + indexOffset;
                indices[i + 5u] = 3u + indexOffset;

                indexOffset += 4;
            }

            m_indexBuffer.Initialise(indices);
        }

        auto QuadBatchState::InitialiseTextureData(const CreateInfo& createInfo) -> void
        {
            m_maxTextureSlots = [maxTextureSlotsInShader = createInfo.maxTextureSlots]()
            {
                GLint maxTextureSlots = 0;
                glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);

                return std::min(static_cast<usize>(maxTextureSlots), maxTextureSlotsInShader);
            }();

            m_textureSlots.resize(m_maxTextureSlots, nullptr);
            m_textureSlots.front() = createInfo.defaultTexture;

            List<Texture::BindingIndex> textureIndices(m_maxTextureSlots);
            std::iota(std::begin(textureIndices), std::end(textureIndices), 0u);

            createInfo.batchPipeline->Use();
            createInfo.batchPipeline->SetTextureUniformVector(createInfo.textureArrayUniformName, textureIndices);
            createInfo.batchPipeline->Disuse();
        }

        auto QuadBatchState::RefreshIfRequired() -> void
        {
            if (m_indexCount >= m_indicesPerBatch || m_currentTextureSlotIndex > m_maxTextureSlots - 1u) [[unlikely]]
            {
                Flush();
                Begin();
            }
        }

        [[nodiscard]] auto QuadBatchState::GetTextureIndex(const Texture& texture) -> usize
        {
            Optional<usize> tentativeTextureIndex = None;

            for (usize i = 0u; i < m_currentTextureSlotIndex; ++i)
            {
                if (m_textureSlots[i] == &texture)
                {
                    tentativeTextureIndex = i;
                }
            }

            if (tentativeTextureIndex.has_value())
            {
                return tentativeTextureIndex.value();
            }

            const usize newTextureIndex = m_currentTextureSlotIndex;

            m_textureSlots[m_currentTextureSlotIndex] = &texture;
            ++m_currentTextureSlotIndex;

            return newTextureIndex;
        }
    }
}
