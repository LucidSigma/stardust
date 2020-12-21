#include "stardust/graphics/renderer/objects/VertexBuffer.h"

#include <utility>

namespace stardust
{
	VertexBuffer::VertexBuffer()
	{
		glGenBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
	}

	VertexBuffer& VertexBuffer::operator =(VertexBuffer&& other) noexcept
	{
		Destroy();
		std::swap(m_id, other.m_id);

		return *this;
	}

	VertexBuffer::~VertexBuffer() noexcept
	{
		Destroy();
	}

	void VertexBuffer::Destroy() noexcept
	{
		Unbind();

		if (m_id != 0u)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0u;
		}
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0u);
	}
}