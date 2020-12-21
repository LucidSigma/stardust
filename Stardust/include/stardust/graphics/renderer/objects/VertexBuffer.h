#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
	class VertexBuffer
	{
	public:
		enum class Usage
			: u32
		{
			Static = GL_STATIC_DRAW,
			Dynamic = GL_DYNAMIC_DRAW,
			Stream = GL_STREAM_DRAW,
		};

	private:
		GLuint m_id = 0u;

	public:
		VertexBuffer();

		template <typename T = f32>
		VertexBuffer(const Vector<T>& vertices, const Usage usage = Usage::Static)
			: VertexBuffer()
		{
			Initialise(vertices, usage);
		}

		~VertexBuffer() noexcept;

		template <typename T = f32>
		void Initialise(const Vector<T>& vertices, const Usage usage = Usage::Static)
		{
			Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), static_cast<GLenum>(usage));
			Unbind();
		}

		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u; }

		void Bind() const;
		void Unbind() const;

		inline u32 GetID() const noexcept { return m_id; }
	};
}

#endif