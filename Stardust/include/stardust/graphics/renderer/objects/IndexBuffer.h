#pragma once
#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <type_traits>

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/BufferUsage.h"

namespace stardust
{
	class IndexBuffer
	{
	private:
		u32 m_id = 0u;
		u32 m_indexCount = 0u;

		u32 m_dataType = GL_UNSIGNED_INT;

	public:
		IndexBuffer();

		template <typename T = u32>
		IndexBuffer(const Vector<T>& indices, const BufferUsage usage = BufferUsage::Static)
			: IndexBuffer()
		{
			Initialise(indices, usage);
		}

		~IndexBuffer() noexcept;

		template <typename T = u32>
		void Initialise(const Vector<T>& indices, const BufferUsage usage = BufferUsage::Static)
		{
			Bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(T) * indices.size(), indices.data(), static_cast<u32>(usage));
			Unbind();

			m_indexCount = static_cast<u32>(indices.size());
			m_dataType = GetGLType<T>();
		}

		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u; }

		void Bind() const;
		void Unbind() const;

		inline u32 GetID() const noexcept { return m_id; }

		inline unsigned int GetIndexCount() const noexcept { return m_indexCount; }
		inline u32 GetDataType() const noexcept { return m_dataType; }

	private:
		template <typename T>
		static constexpr u32 GetGLType()
		{
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