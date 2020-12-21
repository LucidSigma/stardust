#include "stardust/graphics/renderer/objects/VertexLayout.h"

#include <utility>

#include "stardust/debug/logging/Log.h"
#include "stardust/math/Math.h"

namespace stardust
{
	VertexLayout::VertexLayout()
	{
		glGenVertexArrays(1, &m_id);
	}

	VertexLayout::VertexLayout(VertexLayout&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
		std::swap(m_vertexBuffers, other.m_vertexBuffers);
		std::swap(m_attributes, other.m_attributes);
		std::swap(m_vertexSize, other.m_vertexSize);
	}

	VertexLayout& VertexLayout::operator =(VertexLayout&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
		std::swap(m_vertexBuffers, other.m_vertexBuffers);
		std::swap(m_attributes, other.m_attributes);
		std::swap(m_vertexSize, other.m_vertexSize);

		return *this;
	}

	VertexLayout::~VertexLayout() noexcept
	{
		Destroy();
	}

	const VertexLayout& VertexLayout::AddAttribute(const Attribute& attribute) const
	{
		m_attributes.push(AttributeState{
			.attribute = attribute,
			.offset = m_vertexSize 
		});
		m_vertexSize += attribute.elementCount * GetDataTypeSize(attribute.dataType);

		return *this;
	}

	const VertexLayout& VertexLayout::AddVertexBuffer(const VertexBuffer& vertexBuffer) const
	{
		m_vertexBuffers.push_back(eastl::cref(vertexBuffer));

		return *this;
	}

	void VertexLayout::Initialise() const
	{
		Bind();

	#ifndef NDEBUG
		{
			GLint maxAttributeCount = 0;
			glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributeCount);

			if (m_attributes.size() > static_cast<usize>(maxAttributeCount))
			{
				Log::EngineError("Max number of vertex attributes exceeded in vertex array {}.", m_id);
			}
		}
	#endif

		for (const auto& vertexBuffer : m_vertexBuffers)
		{
			vertexBuffer.get().Bind();
		}

		u32 currentVertexLocation = 0u;

		while (!m_attributes.empty())
		{
			constexpr GLuint MaxVertexInputElementCount = 4u;

			const auto& [attribute, offset] = m_attributes.front();
			const u32 vertexInputSlotCount = static_cast<u32>(glm::ceil(
				static_cast<f32>(attribute.elementCount) / static_cast<f32>(MaxVertexInputElementCount)
			));

			for (u32 i = 0; i < vertexInputSlotCount; ++i)
			{
				glVertexAttribPointer(
					currentVertexLocation,
					attribute.elementCount,
					attribute.dataType,
					attribute.isNormalised ? GL_FALSE : GL_TRUE,
					static_cast<GLsizei>(m_vertexSize),
					reinterpret_cast<const GLvoid*>(offset)
				);

				glEnableVertexAttribArray(currentVertexLocation);
				++currentVertexLocation;
			}

			m_attributes.pop();
		}

		for (const auto& vertexBuffer : m_vertexBuffers)
		{
			vertexBuffer.get().Unbind();
		}

		m_vertexBuffers.clear();
		Unbind();
	}

	void VertexLayout::Destroy() noexcept
	{
		Unbind();

		if (m_id != 0u)
		{
			glDeleteVertexArrays(1, &m_id);
			m_id = 0u;

			m_vertexBuffers.clear();
			m_attributes = { };
			m_vertexSize = 0u;
		}
	}

	void VertexLayout::Bind() const
	{
		glBindVertexArray(m_id);
	}

	void VertexLayout::Unbind() const
	{
		glBindVertexArray(0u);
	}

	void VertexLayout::Draw(const u32 count, const u32 offset, const DrawMode drawMode) const
	{
		glDrawArrays(static_cast<GLenum>(drawMode), offset, count);
	}

	void VertexLayout::DrawIndexed(const IndexBuffer& indexBuffer, const bool bindIndexBuffer, const DrawMode drawMode) const
	{
		if (bindIndexBuffer)
		{
			indexBuffer.Bind();
		}

		glDrawElements(static_cast<GLenum>(drawMode), indexBuffer.GetIndexCount(), indexBuffer.GetDataType(), nullptr);

		if (bindIndexBuffer)
		{
			indexBuffer.Unbind();
		}
	}

	usize VertexLayout::GetDataTypeSize(const GLenum dataType)
	{
		static const HashMap<GLenum, usize> sizeLookup{
			{ GL_FLOAT, sizeof(GLfloat) },
			{ GL_INT, sizeof(GLint) },
			{ GL_UNSIGNED_INT, sizeof(GLuint) },
			{ GL_SHORT, sizeof(GLshort) },
			{ GL_UNSIGNED_SHORT, sizeof(GLushort) },
			{ GL_BYTE, sizeof(GLbyte) },
			{ GL_UNSIGNED_BYTE, sizeof(GLubyte) },
		};

		return sizeLookup.at(dataType);
	}
}