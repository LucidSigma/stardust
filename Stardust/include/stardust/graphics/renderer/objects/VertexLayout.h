#pragma once
#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/renderer/objects/IndexBuffer.h"
#include "stardust/graphics/renderer/objects/VertexBuffer.h"

namespace stardust
{
	class VertexLayout
		: private INoncopyable
	{
	public:
		struct Attribute
		{
			u32 elementCount;
			GLenum dataType;
			bool isNormalised;
		};

		enum class DrawMode
			: GLenum
		{
			Points = GL_POINTS,
			Lines = GL_LINES,
			LineLoop = GL_LINE_LOOP,
			LineStrip = GL_LINE_STRIP,
			Triangles = GL_TRIANGLES,
			TriangleStrip = GL_TRIANGLE_STRIP,
			TriangleFan = GL_TRIANGLE_FAN,
			Patches = GL_PATCHES,
		};

	private:
		struct AttributeState
		{
			Attribute attribute;
			usize offset;
		};

		GLuint m_id = 0u;

		mutable Vector<ReferenceWrapper<const VertexBuffer>> m_vertexBuffers{ };
		mutable Queue<AttributeState> m_attributes{ };
		mutable usize m_vertexSize = 0u;

	public:
		VertexLayout();

		VertexLayout(VertexLayout&& other) noexcept;
		VertexLayout& operator =(VertexLayout&& other) noexcept;

		~VertexLayout() noexcept;

		const VertexLayout& AddAttribute(const Attribute& attribute) const;
		const VertexLayout& AddVertexBuffer(const VertexBuffer& vertexBuffer) const;
		void Initialise() const;

		void Destroy() noexcept;

		inline bool IsValid() const noexcept { return m_id != 0u; }

		void Bind() const;
		void Unbind() const;

		void Draw(const u32 count, const u32 offset = 0u, const DrawMode drawMode = DrawMode::Triangles) const;
		void DrawIndexed(const IndexBuffer& indexBuffer, const bool bindIndexBuffer = true, const DrawMode drawMode = DrawMode::Triangles) const;

		inline u32 GetID() const noexcept { return static_cast<u32>(m_id); }

	private:
		static usize GetDataTypeSize(const GLenum dataType);
	};
}

#endif