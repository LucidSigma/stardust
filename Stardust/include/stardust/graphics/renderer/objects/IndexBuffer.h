#pragma once
#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <type_traits>

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/BufferUsage.h"

namespace stardust
{
	class IndexBuffer
		: private INoncopyable
	{
	private:
		GLuint m_id = 0u;
		u32 m_indexCount = 0u;

		GLenum m_dataType = GL_UNSIGNED_INT;

	public:
		IndexBuffer() = default;

		template <typename T = u32>
		IndexBuffer(const Vector<T>& indices, const BufferUsage usage = BufferUsage::Static)
		{
			Initialise(indices, usage);
		}

		IndexBuffer(IndexBuffer&& other) noexcept;
		IndexBuffer& operator =(IndexBuffer&& other) noexcept;

		~IndexBuffer() noexcept;

		template <typename T = u32>
		void Initialise(const Vector<T>& indices, const BufferUsage usage = BufferUsage::Static)
		{
			glGenBuffers(1, &m_id);
			Bind();

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * indices.size(), indices.data(), static_cast<GLenum>(usage));
			
			Unbind();

			m_indexCount = static_cast<u32>(indices.size());
			m_dataType = GetGLType<T>();
		}

		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u; }

		void Bind() const;
		void Unbind() const;

		inline u32 GetID() const noexcept { return static_cast<u32>(m_id); }

		inline unsigned int GetIndexCount() const noexcept { return m_indexCount; }
		inline u32 GetDataType() const noexcept { return static_cast<u32>(m_dataType); }

	private:
		template <typename T>
		static constexpr GLenum GetGLType()
		{
			static_assert(sizeof(u8) == sizeof(GLubyte));
			static_assert(sizeof(u16) == sizeof(GLushort));
			static_assert(sizeof(u32) == sizeof(GLuint));

			if constexpr (std::is_same_v<T, u32>)
			{
				return GL_UNSIGNED_INT;
			}
			else if constexpr (std::is_same_v<T, u16>)
			{
				return GL_UNSIGNED_SHORT;
			}
			else if constexpr (std::is_same_v<T, u8>)
			{
				return GL_UNSIGNED_BYTE;
			}
			else
			{
				static_assert(false, "Unimplemented index buffer type.");
			}
		}
	};
}

#endif