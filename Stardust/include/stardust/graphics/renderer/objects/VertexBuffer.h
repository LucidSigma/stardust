#pragma once
#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/BufferUsage.h"


namespace stardust
{
	class VertexBuffer
	{
	private:
		u32 m_id = 0u;

	public:
		VertexBuffer();

		template <typename T = f32>
		VertexBuffer(const Vector<T>& vertices, const BufferUsage usage = BufferUsage::Static)
			: VertexBuffer()
		{
			Initialise(vertices, usage);
		}

		~VertexBuffer() noexcept;

		template <typename T = f32>
		void Initialise(const Vector<T>& vertices, const BufferUsage usage = BufferUsage::Static)
		{
			Bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), static_cast<u32>(usage));
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