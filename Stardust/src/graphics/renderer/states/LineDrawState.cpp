#include "stardust/graphics/renderer/states/LineDrawState.h"

#include <utility>

#include "stardust/graphics/renderer/objects/BufferUsage.h"
#include "stardust/graphics/renderer/objects/VertexAttribute.h"
#include "stardust/graphics/renderer/objects/VertexLayoutBuilder.h"
#include "stardust/graphics/renderer/objects/Vertices.h"

namespace stardust
{
    namespace graphics
    {
        LineDrawState::LineDrawState(LineDrawState&& other) noexcept
        {
            Destroy();

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);
        }
        
        auto LineDrawState::operator =(LineDrawState&& other) noexcept -> LineDrawState&
        {
            Destroy();

            m_vertexLayout = std::move(other.m_vertexLayout);
            m_vertexBuffer = std::move(other.m_vertexBuffer);

            return *this;
        }
        
        LineDrawState::~LineDrawState() noexcept
        {
            Destroy();
        }

        auto LineDrawState::Initialise() -> void
        {
            m_vertexBuffer.Initialise(2u * sizeof(LineVertex), BufferUsage::Dynamic);

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
                .AddVertexBuffer(m_vertexBuffer)
                .Build();
        }

        auto LineDrawState::Destroy() noexcept -> void
        {
            m_vertexLayout.Destroy();
            m_vertexBuffer.Destroy();
        }

        auto LineDrawState::DrawLine(const geometry::Line& line, const Matrix4& modelMatrix, const Colour& colour) -> void
        {
            m_vertexBuffer.SetSubData<LineVertex>(
                {
                    LineVertex{
                        .position = Vector2(modelMatrix * Vector4{ line.pointA, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(colour),
                    },
                    LineVertex{
                        .position = Vector2(modelMatrix * Vector4{ line.pointB, 0.0f, 1.0f }),
                        .colour = static_cast<Vector4>(colour),
                    },
                },
                2u
            );

            m_vertexLayout.Bind();
            m_vertexLayout.Draw(2u, 0u, DrawMode::Lines);
            m_vertexLayout.Unbind();
        }

        auto LineDrawState::DrawScreenLine(const geometry::ScreenLine& line, const Matrix4& modelMatrix, const Colour& colour) -> void
        {
            DrawLine(
                geometry::Line{
                    .pointA = Vector2(line.pointA),
                    .pointB = Vector2(line.pointB),
                },
                modelMatrix,
                colour
            );
        }
    }
}
