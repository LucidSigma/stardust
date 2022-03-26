#include "stardust/graphics/renderer/objects/VertexLayoutBuilder.h"

#include <functional>

#include <ANGLE/GLES3/gl3.h>

#include "stardust/debug/logging/Logging.h"
#include "stardust/graphics/renderer/objects/VertexLayout.h"
#include "stardust/math/Math.h"

namespace stardust
{
    namespace graphics
    {
        auto VertexLayoutBuilder::AddAttribute(const VertexAttribute& attribute) -> VertexLayoutBuilder&
        {
            m_attributes.push(
                AttributeState{
                    .attribute = attribute,
                    .offset = m_vertexSize,
                }
            );

            m_vertexSize += attribute.elementCount * GetAttributeTypeSize(attribute.dataType);

            return *this;
        }

        auto VertexLayoutBuilder::AddVertexBuffer(const VertexBuffer& vertexBuffer) -> VertexLayoutBuilder&
        {
            m_vertexBuffers.push_back(std::cref(vertexBuffer));

            return *this;
        }

        [[nodiscard]] auto VertexLayoutBuilder::Build() -> VertexLayout
        {
            VertexLayout::ID vertexLayoutID = VertexLayout::InvalidID();
            glGenVertexArrays(1, &vertexLayoutID);

        #ifndef NDEBUG
            {
                GLint maxVertexAttributeCount = 0;
                glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributeCount);

                if (m_attributes.size() > static_cast<usize>(maxVertexAttributeCount))
                {
                    Log::EngineWarn("Max number of vertex attributes ({}) exceeded in vertex array {} (had {} vertex attributes).", maxVertexAttributeCount, vertexLayoutID, m_attributes.size());
                }
            }
        #endif

            glBindVertexArray(vertexLayoutID);

            for (const auto& vertexBuffer : m_vertexBuffers)
            {
                vertexBuffer.get().Bind();
            }

            u32 currentVertexLocation = 0u;
            Queue<u32> enabledVertexLocations{ };

            while (!m_attributes.empty())
            {
                constexpr u32 MaxVertexInputElementCount = 4u;

                const auto& [attribute, offset] = m_attributes.front();
                const u32 vertexInputSlotCount = static_cast<u32>(
                    glm::ceil(
                        static_cast<f32>(attribute.elementCount) / static_cast<f32>(MaxVertexInputElementCount)
                    )
                );

                for (u32 i = 0u; i < vertexInputSlotCount; ++i)
                {
                    glVertexAttribPointer(
                        static_cast<GLuint>(currentVertexLocation),
                        static_cast<GLint>(attribute.elementCount),
                        static_cast<GLenum>(attribute.dataType),
                        attribute.isNormalised ? GL_FALSE : GL_TRUE,
                        static_cast<GLsizei>(m_vertexSize),
                        reinterpret_cast<const void*>(offset)
                    );

                    glEnableVertexAttribArray(currentVertexLocation);
                    enabledVertexLocations.push(currentVertexLocation);

                    ++currentVertexLocation;
                }

                m_attributes.pop();
            }

            glBindVertexArray(VertexLayout::InvalidID());

            while (!enabledVertexLocations.empty())
            {
                glDisableVertexAttribArray(enabledVertexLocations.front());
                enabledVertexLocations.pop();
            }

            for (const auto& vertexBuffer : m_vertexBuffers)
            {
                vertexBuffer.get().Unbind();
            }

            m_vertexBuffers.clear();

            VertexLayout vertexLayout;
            vertexLayout.m_id = vertexLayoutID;

            return vertexLayout;
        }
    }
}
