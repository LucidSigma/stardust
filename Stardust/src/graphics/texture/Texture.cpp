#include "stardust/graphics/texture/Texture.h"

#include <utility>

#include <stb_image/stb_image.h>

#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/graphics/Colour.h"
#include "stardust/math/Math.h"

namespace stardust
{
	void Texture::ResetActiveTexture()
	{
		glActiveTexture(GL_TEXTURE0);
	}

	Texture::Texture(const StringView& filepath, const Sampler& sampler)
	{
		Initialise(filepath, sampler);
	}

	Texture::Texture(const Vector<ubyte>& data, const UVec2& extent, const u32 channelCount, const Sampler& sampler)
	{
		Initialise(data, extent, channelCount, sampler);
	}

	Texture::Texture(const Vec2& size, const Sampler& sampler)
	{
		Initialise(size, sampler);
	}

	Texture::Texture(Texture&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
		std::swap(m_size, other.m_size);
		std::swap(m_isValid, other.m_isValid);
	}

	Texture& Texture::operator =(Texture&& other) noexcept
	{
		Destroy();

		std::swap(m_id, other.m_id);
		std::swap(m_size, other.m_size);
		std::swap(m_isValid, other.m_isValid);

		return *this;
	}

	Texture::~Texture() noexcept
	{
		Destroy();
	}

	void Texture::Initialise(const StringView& filepath, const Sampler& sampler)
	{
		if (s_maxTextureUnits == 0)
		{
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_maxTextureUnits);
		}

		glGenTextures(1, &m_id);
		m_isValid = LoadFromImageFile(filepath, sampler) == Status::Success;
	}

	void Texture::Initialise(const Vector<ubyte>& data, const UVec2& extent, const u32 channelCount, const Sampler& sampler)
	{
		if (s_maxTextureUnits == 0)
		{
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_maxTextureUnits);
		}

		glGenTextures(1, &m_id);

		constexpr i32 DefaultPixelAlignment = 4;

		if (const i32 imageMemorySize = static_cast<i32>(extent.x * channelCount * extent.y);
			imageMemorySize % DefaultPixelAlignment != 0)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, imageMemorySize % 2 == 0 ? 2 : 1);
		}

		m_size.x = extent.x;
		m_size.y = extent.y;
		const auto [internalFormat, format] = s_componentMap.at(static_cast<i32>(channelCount));

		SetupParameters(internalFormat, format, data.data(), sampler);

		glPixelStorei(GL_UNPACK_ALIGNMENT, DefaultPixelAlignment);
	}

	void Texture::Initialise(const Vec2& size, const Sampler& sampler)
	{
		if (s_maxTextureUnits == 0)
		{
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &s_maxTextureUnits);
		}

		glGenTextures(1, &m_id);
		m_size = size;

		SetupParameters(GL_RGB8, GL_RGB, nullptr, sampler);
		m_isValid = true;
	}

	void Texture::Destroy() noexcept
	{
		Unbind();

		if (m_id != 0u)
		{
			glDeleteTextures(1, &m_id);
			m_id = 0u;

			m_size = UVec2{ 0u, 0u };
			m_isValid = false;
		}
	}

	void Texture::Bind(const i32 index) const
	{
	#ifndef NDEBUG
		if (index >= s_maxTextureUnits)
		{
			Log::EngineWarn("Texture {} is bound to index {} - exceeding max supported texture units.", m_id, index);
		}
	#endif

		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0u);
	}

	[[nodiscard]] Status Texture::LoadFromImageFile(const StringView& filepath, const Sampler& sampler)
	{
		const Vector<ubyte> rawTextureData = vfs::ReadFileData(filepath);

		if (rawTextureData.empty())
		{
			return Status::Fail;
		}

		i32 width = 0;
		i32 height = 0;
		i32 componentCount = 0;

		ubyte* data = reinterpret_cast<ubyte*>(stbi_load_from_memory(
			reinterpret_cast<const stbi_uc*>(rawTextureData.data()),
			static_cast<i32>(rawTextureData.size()),
			&width, &height, &componentCount,
			STBI_default
		));

		if (data == nullptr)
		{
			return Status::Fail;
		}

		constexpr i32 DefaultPixelAlignment = 4;

		if (const i32 imageMemorySize = width * componentCount * height;
			imageMemorySize % DefaultPixelAlignment != 0)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, imageMemorySize % 2 == 0 ? 2 : 1);
		}

		m_size.x = static_cast<u32>(width);
		m_size.y = static_cast<u32>(height);
		const auto [internalFormat, format] = s_componentMap.at(static_cast<i32>(componentCount));

		SetupParameters(internalFormat, format, data, sampler);

		stbi_image_free(data);
		data = nullptr;

		glPixelStorei(GL_UNPACK_ALIGNMENT, DefaultPixelAlignment);

		return Status::Success;
	}

	void Texture::SetupParameters(const GLint internalFormat, const GLenum format, const ubyte* data, const Sampler& sampler)
	{
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_size.x, m_size.y, 0, format, GL_UNSIGNED_BYTE, data);

		if (sampler.generateMipmaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		const Array<Pair<GLenum, GLint>, 4u> textureParameters{
			Pair<GLenum, GLint>{ GL_TEXTURE_WRAP_S, static_cast<GLint>(sampler.sWrap) },
			Pair<GLenum, GLint>{ GL_TEXTURE_WRAP_T, static_cast<GLint>(sampler.tWrap) },
			Pair<GLenum, GLint>{ GL_TEXTURE_MIN_FILTER, static_cast<GLint>(sampler.minFilter) },
			Pair<GLenum, GLint>{ GL_TEXTURE_MAG_FILTER, static_cast<GLint>(sampler.magFilter) },
		};

		for (const auto [parameter, value] : textureParameters)
		{
			glTexParameteri(GL_TEXTURE_2D, parameter, value);
		}

		if (sampler.sWrap == Sampler::Wrap::ClampToBorder || sampler.tWrap == Sampler::Wrap::ClampToBorder)
		{
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(ColourToVec4(sampler.borderColour.value())));
		}

		if (sampler.enableAnisotropicFiltering)
		{
			f32 maxAnisotropy = 0.0f;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);
		}

		Unbind();
	}
}