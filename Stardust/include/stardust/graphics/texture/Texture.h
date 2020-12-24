#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/utility/enums/Status.h"

namespace stardust
{
	class Texture
		: private INoncopyable
	{
	private:
		inline static const HashMap<u32, Pair<GLint, GLenum>> s_componentMap{
			{ 1u, { GL_R8, GL_RED } },
			{ 2u, { GL_RG8, GL_RG } },
			{ 3u, { GL_RGB8, GL_RGB } },
			{ 4u, { GL_RGBA8, GL_RGBA } },
		};

		inline static GLint s_maxTextureUnits = 0;

		GLuint m_id = 0u;
		UVec2 m_size{ 0u, 0u };

		bool m_isValid = false;

	public:
		static void ResetActiveTexture();
		inline static u32 GetMaxTextureUnits() { return static_cast<u32>(s_maxTextureUnits); }

		Texture() = default;
		Texture(const StringView& filepath, const Sampler& sampler = Sampler{ });
		Texture(const Vec2& size, const Sampler& sampler = Sampler{ });

		Texture(Texture&& other) noexcept;
		Texture& operator =(Texture&& other) noexcept;

		~Texture() noexcept;

		void Initialise(const StringView& filepath, const Sampler& sampler = Sampler{ });
		void Initialise(const Vec2& size, const Sampler& sampler = Sampler{ });
		void Destroy() noexcept;

		void Bind(const int index = 0) const;
		void Unbind() const;

		inline bool IsValid() const noexcept { return m_id != 0 && m_isValid; }

		inline GLuint GetID() const noexcept { return m_id; }
		inline const UVec2& GetSize() const noexcept { return m_size; }

	private:
		Status LoadFromImageFile(const StringView& filepath, const Sampler& sampler);
		void SetupParameters(const GLint internalFormat, const GLenum format, const ubyte* data, const Sampler& sampler);
	};
}

#endif