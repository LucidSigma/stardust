#include "stardust/graphics/renderer/states/LineBatchState.h"

#include <utility>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/graphics/renderer/objects/VertexAttribute.h"
#include "stardust/graphics/renderer/objects/VertexLayoutBuilder.h"
#include "stardust/graphics/renderer/objects/Vertices.h"

namespace stardust
{
    namespace graphics
    {
        LineBatchState::LineBatchState(const usize maxShapesPerBatch)
        {
            Initialise(maxShapesPerBatch);
        }

        LineBatchState::LineBatchState(LineBatchState&& other) noexcept
        {
            Destroy();

            m_lineBuffer = std::move(other.m_lineBuffer);
            m_bufferOffset = std::exchange(other.m_bufferOffset, nullptr);

            m_vertexCount = std::exchange(other.m_vertexCount, 0u);

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);
        }

        auto LineBatchState::operator =(LineBatchState&& other) noexcept -> LineBatchState&
        {
            Destroy();

            m_lineBuffer = std::move(other.m_lineBuffer);
            m_bufferOffset = std::exchange(other.m_bufferOffset, nullptr);

            m_vertexCount = std::exchange(other.m_vertexCount, 0u);

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);

            return *this;
        }
        
        LineBatchState::~LineBatchState() noexcept
        {
            Destroy();
        }
        
        auto LineBatchState::Initialise(const usize maxShapesPerBatch) -> void
        {
            m_verticesPerBatch = maxShapesPerBatch * 2u;
            InitialiseRenderObjects();

            m_lineBuffer.resize(m_verticesPerBatch);
            m_bufferOffset = m_lineBuffer.data();
        }

        auto LineBatchState::Destroy() noexcept -> void
        {
            m_vertexLayout.Destroy();
            m_vertexBuffer.Destroy();
        }

        auto LineBatchState::Begin() -> void
        {
            m_bufferOffset = m_lineBuffer.data();
            m_vertexCount = 0u;
        }
        
        auto LineBatchState::Flush() -> void
        {
            const isize batchSize = m_bufferOffset - m_lineBuffer.data();
            m_vertexBuffer.SetSubData(m_lineBuffer, static_cast<usize>(batchSize));

            m_vertexLayout.Bind();
            m_vertexLayout.Draw(m_vertexCount, 0u, DrawMode::Lines);
            m_vertexLayout.Unbind();
        }

        auto LineBatchState::BatchLine(const geometry::Line& line, const Matrix4& modelMatrix, const Colour& colour) -> void
        {
            RefreshIfRequired();

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ line.pointA, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(colour);
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ line.pointB, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(colour);
            m_bufferOffset->projectionType = ViewProjectionType;
            ++m_bufferOffset;

            m_vertexCount += 2u;
        }

        auto LineBatchState::BatchScreenLine(const geometry::ScreenLine& line, const Matrix4& modelMatrix, const Colour& colour) -> void
        {
            RefreshIfRequired();

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ line.pointA, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(colour);
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_bufferOffset->position = Vector2(modelMatrix * Vector4{ line.pointB, 0.0f, 1.0f });
            m_bufferOffset->colour = static_cast<Vector4>(colour);
            m_bufferOffset->projectionType = ScreenProjectionType;
            ++m_bufferOffset;

            m_vertexCount += 2u;
        }
        
        auto LineBatchState::InitialiseRenderObjects() -> void
        {
            m_vertexBuffer.Initialise(m_verticesPerBatch * sizeof(BatchLineVertex), BufferUsage::Dynamic);

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
                    .elementCount = 1u,
                    .dataType = VertexAttribute::Type::Float32,
                    .isNormalised = true,
                })
                .AddVertexBuffer(m_vertexBuffer)
                .Build();
        }
        
        auto LineBatchState::RefreshIfRequired() -> void
        {
            if (m_vertexCount >= m_verticesPerBatch) [[unlikely]]
            {
                Flush();
                Begin();
            }
        }
    }
}
