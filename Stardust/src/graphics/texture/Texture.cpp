#include "stardust/graphics/texture/Texture.h"

#include <cstring>
#include <utility>

#include <stb/stb_image.h>

#include "stardust/debug/logging/Log.h"
#include "stardust/filesystem/vfs/VFS.h"
#include "stardust/graphics/Colour.h"

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

    Texture::Texture(SDL_Surface* surface, const bool flipVertically, const Sampler& sampler)
    {
        Initialise(surface, flipVertically, sampler);
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
        glGenTextures(1, &m_id);
        m_isValid = LoadFromImageFile(filepath, sampler) == Status::Success;
    }

    void Texture::Initialise(SDL_Surface* surface, const bool flipVertically, const Sampler& sampler)
    {
        glGenTextures(1, &m_id);
        m_isValid = LoadFromSDLSurface(surface, flipVertically, sampler) == Status::Success;
    }

    void Texture::Initialise(const Vector<ubyte>& data, const UVec2& extent, const u32 channelCount, const Sampler& sampler)
    {
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
        m_isValid = true;
    }

    void Texture::Initialise(const Vec2& size, const Sampler& sampler)
    {
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

            m_size = UVec2Zero;
            m_isValid = false;
        }
    }

    void Texture::Bind(const i32 index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::Unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0u);
    }

    [[nodiscard]] SDL_Surface* Texture::FlipSurface(const SDL_Surface* const surface)
    {
        SDL_Surface* flippedSurface = SDL_CreateRGBSurface(
            surface->flags, surface->w, surface->h, static_cast<i32>(surface->format->BytesPerPixel * 8u),
            surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask
        );

        if (flippedSurface == nullptr)
        {
            return nullptr;
        }

        const i32 distanceToLastRow = surface->pitch * (surface->h - 1);
        ubyte* currentDestinationRow = static_cast<ubyte*>(surface->pixels) + distanceToLastRow;
        ubyte* currentSourceRow = static_cast<ubyte*>(flippedSurface->pixels);

        for (u32 line = 0u; line < static_cast<u32>(surface->h); ++line)
        {
            std::memcpy(currentSourceRow, currentDestinationRow, surface->pitch);

            currentDestinationRow -= surface->pitch;
            currentSourceRow += surface->pitch;
        }

        return flippedSurface;
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

    Status Texture::LoadFromSDLSurface(SDL_Surface* surface, const bool flipVertically, const Sampler& sampler)
    {
        SDL_Surface* targetSurface = surface;

        if (flipVertically)
        {
            targetSurface = FlipSurface(surface);

            if (targetSurface == nullptr)
            {
                return Status::Fail;
            }
        }

        constexpr i32 DefaultPixelAlignment = 4;

        if (const i32 imageMemorySize = static_cast<i32>(targetSurface->pitch * targetSurface->h);
            imageMemorySize % DefaultPixelAlignment != 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, imageMemorySize % 2 == 0 ? 2 : 1);
        }

        m_size.x = targetSurface->w;
        m_size.y = targetSurface->h;
        auto [internalFormat, format] = s_componentMap.at(targetSurface->format->BytesPerPixel);

        if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
            if (targetSurface->format->format == SDL_PIXELFORMAT_RGB888)
            {
                format = GL_BGR;
            }
            else if (targetSurface->format->format == SDL_PIXELFORMAT_ARGB8888)
            {
                format = GL_BGRA;
            }
        }

        SetupParameters(internalFormat, format, reinterpret_cast<const ubyte*>(targetSurface->pixels), sampler);

        if (flipVertically)
        {
            SDL_FreeSurface(targetSurface);
            targetSurface = nullptr;
        }

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
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(Vec4(sampler.borderColour.value())));
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