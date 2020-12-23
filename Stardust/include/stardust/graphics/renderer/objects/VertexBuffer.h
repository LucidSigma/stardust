#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/BufferUsage.h"

namespace stardust
{
	class VertexBuffer
		: private INoncopyable
	{
	private:
		GLuint m_id = 0u;

	public:
		VertexBuffer() = default;

		template <typename T = f32>
		VertexBuffer(const Vector<T>& vertices, const BufferUsage usage = BufferUsage::Static)
		{
			Initialise(vertices, usage);
		}

		VertexBuffer(VertexBuffer&& other) noexcept;
		VertexBuffer& operator =(VertexBuffer&& other) noexcept;

		~VertexBuffer() noexcept;

		template <typename T = f32>
		void Initialise(const Vector<T>& vertices, const BufferUsage usage = BufferUsage::Static)
		{
			glGenBuffers(1, &m_id);
			Bind();

			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), static_cast<GLenum>(usage));
			
			Unbind();
		}

		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u; }

		void Bind() const;
		void Unbind() const;

		inline u32 GetID() const noexcept { return static_cast<u32>(m_id); }
	};
}

#endif