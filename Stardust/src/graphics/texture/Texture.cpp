#include "stardust/graphics/texture/Texture.h"

#include <utility>

#include <stb/stb_image.h>

#include "stardust/debug/logging/Logging.h"
#include "stardust/filesystem/vfs/VirtualFilesystem.h"

namespace stardust
{
    namespace graphics
    {
        auto TextureCoordinatePair::Inset(const Texture& texture, const f32 texelCount) -> void
        {
            const Vector2 halfTexelSize = 0.5f * texelCount / Vector2(texture.GetSize());

            lowerLeft += halfTexelSize;
            upperRight -= halfTexelSize;
        }

        auto TextureCoordinatePair::Inset(const Texture& texture, const Vector2 texelCount) -> void
        {
            Inset(texture, texelCount.x, texelCount.y);
        }

        auto TextureCoordinatePair::Inset(const Texture& texture, const f32 uTexelCount, const f32 vTexelCount) -> void
        {
            const f32 uTexelSize = uTexelCount / static_cast<f32>(texture.GetSize().x);
            const f32 vTexelSize = vTexelCount / static_cast<f32>(texture.GetSize().y);
            const Vector2 texelSize{ uTexelSize, vTexelSize };

            lowerLeft += texelSize;
            upperRight -= texelSize;
        }

        auto TextureCoordinatePair::ShiftLowerLeft(const Texture& texture, const f32 texelCount) -> void
        {
            const Vector2 texelSize = texelCount / Vector2(texture.GetSize());

            lowerLeft += texelSize;
        }

        auto TextureCoordinatePair::ShiftLowerLeft(const Texture& texture, const Vector2 texelCount) -> void
        {
            ShiftLowerLeft(texture, texelCount.x, texelCount.y);
        }

        auto TextureCoordinatePair::ShiftLowerLeft(const Texture& texture, const f32 uTexelCount, const f32 vTexelCount) -> void
        {
            const f32 uTexelSize = uTexelCount / static_cast<f32>(texture.GetSize().x);
            const f32 vTexelSize = vTexelCount / static_cast<f32>(texture.GetSize().y);
            const Vector2 texelSize{ uTexelSize, vTexelSize };

            lowerLeft += texelSize;
        }

        auto TextureCoordinatePair::ShiftUpperRight(const Texture& texture, const f32 texelCount) -> void
        {
            const Vector2 texelSize = texelCount / Vector2(texture.GetSize());

            upperRight += texelSize;
        }

        auto TextureCoordinatePair::ShiftUpperRight(const Texture& texture, const Vector2 texelCount) -> void
        {
            ShiftUpperRight(texture, texelCount.x, texelCount.y);
        }

        auto TextureCoordinatePair::ShiftUpperRight(const Texture& texture, const f32 uTexelCount, const f32 vTexelCount) -> void
        {
            const f32 uTexelSize = uTexelCount / static_cast<f32>(texture.GetSize().x);
            const f32 vTexelSize = vTexelCount / static_cast<f32>(texture.GetSize().y);
            const Vector2 texelSize{ uTexelSize, vTexelSize };

            upperRight += texelSize;
        }

        auto Texture::ResetActiveTexture() -> void
        {
            glActiveTexture(GL_TEXTURE0);
        }

        Texture::Texture(const StringView filepath, const Sampler& sampler)
        {
            Initialise(filepath, sampler);
        }

        Texture::Texture(const UVector2 extent, const u32 channelCount, const Sampler& sampler)
        {
            Initialise(extent, channelCount, sampler);
        }

        Texture::Texture(const List<ubyte>& data, const UVector2 extent, const u32 channelCount, const Sampler& sampler)
        {
            Initialise(data, extent, channelCount, sampler);
        }

        Texture::Texture(const ubyte* const data, const UVector2 extent, const u32 channelCount, const Sampler& sampler)
        {
            Initialise(data, extent, channelCount, sampler);
        }

        Texture::Texture(Texture&& other) noexcept
        {
            Destroy();

            std::swap(m_id, other.m_id);
            std::swap(m_size, other.m_size);
            std::swap(m_isValid, other.m_isValid);
        }

        auto Texture::operator =(Texture&& other) noexcept -> Texture&
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

        auto Texture::Initialise(const StringView filepath, const Sampler& sampler) -> void
        {
            glGenTextures(1, &m_id);
            m_isValid = LoadFromImageFile(filepath, sampler) == Status::Success;
        }

        auto Texture::Initialise(const UVector2 extent, const u32 channelCount, const Sampler& sampler) -> void
        {
            Initialise(nullptr, extent, channelCount, sampler);
        }

        auto Texture::Initialise(const List<ubyte>& data, const UVector2 extent, const u32 channelCount, const Sampler& sampler) -> void
        {
            Initialise(data.data(), extent, channelCount, sampler);
        }

        auto Texture::Initialise(const ubyte* const data, const UVector2 extent, const u32 channelCount, const Sampler& sampler) -> void
        {
            glGenTextures(1, &m_id);

            constexpr i32 DefaultPixelAlignment = 4;

            if (const i32 imageMemorySize = static_cast<i32>(extent.x * channelCount * extent.y);
                imageMemorySize % DefaultPixelAlignment != 0)
            {
                glPixelStorei(GL_UNPACK_ALIGNMENT, imageMemorySize % 2 == 0 ? 2 : 1);
            }

            m_size = extent;
            const auto [internalComponentFormat, pixelFormat] = s_componentMap.at(channelCount);

            SetupParameters(internalComponentFormat, pixelFormat, data, sampler);

            glPixelStorei(GL_UNPACK_ALIGNMENT, DefaultPixelAlignment);
            m_isValid = true;
        }

        auto Texture::Destroy() noexcept -> void
        {
            if (m_id != s_InvalidID)
            {
                Unbind();

                glDeleteTextures(1, &m_id);

                m_id = s_InvalidID;
                m_size = UVector2Zero;
                m_isValid = false;
            }
        }

        auto Texture::Bind(const BindingIndex bindingIndex) const -> void
        {
        #ifndef NDEBUG
            {
                GLint maxCombinedTextureImageUnitCount = 0;
                glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnitCount);

                if (bindingIndex >= static_cast<BindingIndex>(maxCombinedTextureImageUnitCount))
                {
                    Log::EngineWarn("Texture {} is bound to index {}, but the implementation doesn't support that many binding indices.", m_id, bindingIndex);
                }
            }
        #endif

            glActiveTexture(GL_TEXTURE0 + static_cast<GLint>(bindingIndex));
            glBindTexture(GL_TEXTURE_2D, m_id);
        }

        auto Texture::Unbind() const -> void
        {
            glBindTexture(GL_TEXTURE_2D, s_InvalidID);
        }

        auto Texture::SetData(const List<ubyte>& data, const UVector2 extent, const u32 channelCount) -> void
        {
            SetData(data.data(), extent, channelCount);
        }

        auto Texture::SetData(const ubyte* const data, const UVector2 extent, const u32 channelCount) -> void
        {
            constexpr i32 DefaultPixelAlignment = 4;

            if (const i32 imageMemorySize = static_cast<i32>(extent.x * channelCount * extent.y);
                imageMemorySize % DefaultPixelAlignment != 0)
            {
                glPixelStorei(GL_UNPACK_ALIGNMENT, imageMemorySize % 2 == 0 ? 2 : 1);
            }

            m_size = extent;
            const auto [internalComponentFormat, pixelFormat] = s_componentMap.at(channelCount);

            Bind();

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                static_cast<GLint>(internalComponentFormat),
                static_cast<GLsizei>(m_size.x),
                static_cast<GLsizei>(m_size.y),
                0,
                static_cast<GLenum>(pixelFormat),
                pixelFormat != PixelFormat::DepthStencil ? GL_UNSIGNED_BYTE : GL_UNSIGNED_INT_24_8,
                data
            );

            Unbind();
            glPixelStorei(GL_UNPACK_ALIGNMENT, DefaultPixelAlignment);
        }

        auto Texture::SetHorizontalWrapMode(const TextureWrap horizontalWrap) const -> void
        {
            Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLint>(horizontalWrap));
            Unbind();
        }

        auto Texture::SetVerticalWrapMode(const TextureWrap verticalWrap) const -> void
        {
            Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLint>(verticalWrap));
            Unbind();
        }

        auto Texture::SetMinFilter(const TextureMinFilter minFilter) const -> void
        {
            Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
            Unbind();
        }

        auto Texture::SetMagFilter(const TextureMagFilter magFilter) const -> void
        {
            Bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
            Unbind();
        }

        [[nodiscard]] auto Texture::GetSizeFromCoordinates(const TextureCoordinatePair& textureCoordinates) const noexcept -> UVector2
        {
            const f32 normalisedWidth = glm::abs(textureCoordinates.upperRight.x - textureCoordinates.lowerLeft.x);
            const f32 normalisedHeight = glm::abs(textureCoordinates.upperRight.y - textureCoordinates.lowerLeft.y);

            return UVector2{
                static_cast<u32>(glm::round(normalisedWidth * GetSize().x)),
                static_cast<u32>(glm::round(normalisedHeight * GetSize().y)),
            };
        }

        [[nodiscard]] auto Texture::GetWidthFromCoordinates(const TextureCoordinatePair& textureCoordinates) const noexcept -> u32
        {
            const f32 normalisedWidth = glm::abs(textureCoordinates.upperRight.x - textureCoordinates.lowerLeft.x);

            return static_cast<u32>(glm::round(normalisedWidth * GetSize().x));
        }

        [[nodiscard]] auto Texture::GetHeightFromCoordinates(const TextureCoordinatePair& textureCoordinates) const noexcept -> u32
        {
            const f32 normalisedHeight = glm::abs(textureCoordinates.upperRight.y - textureCoordinates.lowerLeft.y);

            return static_cast<u32>(glm::round(normalisedHeight * GetSize().y));
        }

        [[nodiscard]] auto Texture::LoadFromImageFile(const StringView filepath, const Sampler& sampler) -> Status
        {
            auto textureFileDataResult = vfs::ReadFileBytes(filepath);

            if (textureFileDataResult.is_err())
            {
                return Status::Fail;
            }

            const List<ubyte> textureFileData = std::move(textureFileDataResult).unwrap();

            i32 width = 0;
            i32 height = 0;
            i32 componentCount = 0;

            stbi_uc* textureData = stbi_load_from_memory(
                reinterpret_cast<const stbi_uc*>(textureFileData.data()),
                static_cast<i32>(textureFileData.size()),
                &width,
                &height,
                &componentCount,
                STBI_default
            );

            if (textureData == nullptr)
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
            const auto [internalComponentFormat, pixelFormat] = s_componentMap.at(static_cast<u32>(componentCount));

            SetupParameters(internalComponentFormat, pixelFormat, reinterpret_cast<const ubyte*>(textureData), sampler);

            stbi_image_free(textureData);
            textureData = nullptr;

            glPixelStorei(GL_UNPACK_ALIGNMENT, DefaultPixelAlignment);

            return Status::Success;
        }

        auto Texture::SetupParameters(const InternalComponentFormat internalComponentFormat, const PixelFormat pixelFormat, const ubyte* const data, const Sampler& sampler) -> void
        {
            Bind();

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                static_cast<GLint>(internalComponentFormat),
                static_cast<GLsizei>(m_size.x),
                static_cast<GLsizei>(m_size.y),
                0,
                static_cast<GLenum>(pixelFormat),
                pixelFormat != PixelFormat::DepthStencil ? GL_UNSIGNED_BYTE : GL_UNSIGNED_INT_24_8,
                data
            );

            if (sampler.generateMipmaps)
            {
                glGenerateMipmap(GL_TEXTURE_2D);
            }

            const Array<Pair<GLenum, GLint>, 4u> textureParameters{
                Pair<GLenum, GLint>{ GL_TEXTURE_WRAP_S, static_cast<GLint>(sampler.horizontalWrap) },
                Pair<GLenum, GLint>{ GL_TEXTURE_WRAP_T, static_cast<GLint>(sampler.verticalWrap) },
                Pair<GLenum, GLint>{ GL_TEXTURE_MIN_FILTER, static_cast<GLint>(sampler.minFilter) },
                Pair<GLenum, GLint>{ GL_TEXTURE_MAG_FILTER, static_cast<GLint>(sampler.magFilter) },
            };

            for (const auto [parameter, value] : textureParameters)
            {
                glTexParameteri(GL_TEXTURE_2D, parameter, value);
            }

            Unbind();
        }
    }
}
