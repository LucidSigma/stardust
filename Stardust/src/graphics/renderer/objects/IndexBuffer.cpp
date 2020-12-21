#include "stardust/graphics/renderer/objects/IndexBuffer.h"

namespace stardust
{
	IndexBuffer::IndexBuffer()
	{
		glGenBuffers(1, &m_id);
	}

	IndexBuffer::~IndexBuffer() noexcept
	{
		Destroy();
	}

	void IndexBuffer::Destroy() noexcept
	{
		Unbind();

		if (m_id != 0u)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0u;
		}
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
	}
}