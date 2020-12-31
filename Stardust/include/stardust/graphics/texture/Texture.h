#pragma once
#ifndef STARDUST_TEXTURE_H
#define STARDUST_TEXTURE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Types.h"
#include "stardust/graphics/texture/Sampler.h"
#include "stardust/utility/status/Status.h"

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

		GLuint m_id = 0u;
		UVec2 m_size{ 0u, 0u };

		bool m_isValid = false;

	public:
		static void ResetActiveTexture();

		Texture() = default;
		explicit Texture(const StringView& filepath, const Sampler& sampler = Sampler{ });
		Texture(SDL_Surface* surface, const bool flipVertically, const Sampler& sampler = Sampler{ });
		Texture(const Vector<ubyte>& data, const UVec2& extent, const u32 channelCount, const Sampler& sampler = Sampler{ });
		explicit Texture(const Vec2& size, const Sampler& sampler = Sampler{ });

		Texture(Texture&& other) noexcept;
		Texture& operator =(Texture&& other) noexcept;

		~Texture() noexcept;

		void Initialise(const StringView& filepath, const Sampler& sampler = Sampler{ });
		void Initialise(SDL_Surface* surface, const bool flipVertically, const Sampler& sampler = Sampler{ });
		void Initialise(const Vector<ubyte>& data, const UVec2& extent, const u32 channelCount, const Sampler& sampler = Sampler{ });
		void Initialise(const Vec2& size, const Sampler& sampler = Sampler{ });
		void Destroy() noexcept;
		
		void Bind(const i32 index = 0) const;
		void Unbind() const;

		inline bool IsValid() const noexcept { return m_id != 0u && m_isValid; }

		inline GLuint GetID() const noexcept { return m_id; }
		inline const UVec2& GetSize() const noexcept { return m_size; }

	private:
		[[nodiscard]] static SDL_Surface* FlipSurface(const SDL_Surface* const surface);

		[[nodiscard]] Status LoadFromImageFile(const StringView& filepath, const Sampler& sampler);
		[[nodiscard]] Status LoadFromSDLSurface(SDL_Surface* surface, const bool flipVertically, const Sampler& sampler);
		void SetupParameters(const GLint internalFormat, const GLenum format, const ubyte* data, const Sampler& sampler);
	};

	using CoordinatePair = Pair<Vec2, Vec2>;
}

#endif