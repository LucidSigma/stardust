#include "stardust/ui/UI.h"

#include <cstdio>
#include <cstring>
#include <utility>

#include <RmlUi/Core.h>

#include "stardust/application/Application.h"
#include "stardust/data/MathTypes.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"
#include "stardust/filesystem/vfs/VirtualFileHandle.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    namespace ui
    {
        namespace
        {
            class SystemInferface
                : public Rml::SystemInterface
            {
            private:
                ObserverPtr<const Application> m_application = nullptr;

            public:
                SystemInferface() = default;
                virtual ~SystemInferface() noexcept = default;

                void Initialise(const Application& application)
                {
                    m_application = &application;
                }

                [[nodiscard]] virtual f64 GetElapsedTime() override
                {
                    return m_application->GetElapsedTime();
                }
            };

            class FileInterface
                : public Rml::FileInterface
            {
            private:
                ObserverPtr<const Application> m_application = nullptr;

                HashMap<Rml::FileHandle, vfs::VirtualFileHandle> m_handles{ };
                u32 m_fileHandleIDCounter = 1u;

            public:
                FileInterface() = default;
                virtual ~FileInterface() noexcept = default;

                void Initialise(const Application& application)
                {
                    m_application = &application;
                }

                [[nodiscard]] virtual Rml::FileHandle Open(const Rml::String& filepath) override
                {
                    const auto [iterator, wasInserted] = m_handles.insert({ Rml::FileHandle(m_fileHandleIDCounter), vfs::VirtualFileHandle() });
                    ++m_fileHandleIDCounter;

                    if (!wasInserted) [[unlikely]]
                    {
                        return Rml::FileHandle(0u);
                    }

                    iterator->second.Open(filepath, vfs::VirtualFileHandle::OpenMode::Read);

                    if (!iterator->second.IsValid())
                    {
                        return Rml::FileHandle(0u);
                    }

                    return iterator->first;
                }

                virtual void Close(const Rml::FileHandle fileHandle) override
                {
                    if (m_handles.contains(fileHandle)) [[likely]]
                    {
                        m_handles[fileHandle].Close();
                    }
                }

                [[nodiscard]] virtual usize Read(void* const buffer, const usize bufferSize, const Rml::FileHandle fileHandle) override
                {
                    if (!m_handles.contains(fileHandle)) [[unlikely]]
                    {
                        return 0u;
                    }

                    const Vector<ubyte> data = m_handles[fileHandle].Read(bufferSize);

                    if (data.empty())
                    {
                        return 0u;
                    }

                    std::memcpy(buffer, data.data(), data.size());

                    return data.size();
                }

                [[nodiscard]] virtual bool Seek(const Rml::FileHandle fileHandle, const long offset, const i32 origin) override
                {
                    if (!m_handles.contains(fileHandle)) [[unlikely]]
                    {
                        return false;
                    }

                    i64 virtualOffset = 0u;

                    switch (origin)
                    {
                    case SEEK_CUR:
                        {
                            const Optional<i64> pointerLocation = m_handles[fileHandle].Tell();

                            if (!pointerLocation.has_value())
                            {
                                return false;
                            }

                            virtualOffset += pointerLocation.value();

                            break;
                        }

                    case SEEK_END:
                        virtualOffset += static_cast<i64>(m_handles[fileHandle].Size());

                    case SEEK_SET:
                    default:
                        break;
                    }

                    virtualOffset += static_cast<i64>(offset);

                    return m_handles[fileHandle].Seek(static_cast<u64>(virtualOffset)) == Status::Success;
                }

                [[nodiscard]] usize Tell(const Rml::FileHandle fileHandle) override
                {
                    if (m_handles.contains(fileHandle)) [[likely]]
                    {
                        const Optional<i64> pointerLocation = m_handles[fileHandle].Tell();

                        return pointerLocation.has_value() ? static_cast<usize>(pointerLocation.value()) : 0u;
                    }

                    return 0u;
                }
            };

            class RenderInterface
                : public Rml::RenderInterface
            {
            private:
                ObserverPtr<const Application> m_application = nullptr;

            public:
                RenderInterface() = default;
                virtual ~RenderInterface() noexcept = default;

                void Initialise(const Application& application)
                {
                    m_application = &application;
                }

                virtual void RenderGeometry(Rml::Vertex* vertices, const i32 vertexCount, i32* const indices, const i32 indexCount, const Rml::TextureHandle textureHandle, const Rml::Vector2f& translation) override
                {

                }

                virtual void EnableScissorRegion(const bool enable) override
                {
                    m_application->GetRenderer().EnableScissorRect(enable);
                }

                virtual void SetScissorRegion(const i32 x, const i32 y, const i32 width, const i32 height) override
                {
                    m_application->GetRenderer().SetScissorRect(IVec2{ x, y }, UVec2{ static_cast<u32>(width), static_cast<u32>(height) });
                }
            };

            SystemInferface systemInterface;
            FileInterface fileInterface;
            RenderInterface renderInterface;
        }
    
        [[nodiscard]] Status Initialise(const Application& application)
        {
            systemInterface.Initialise(application);
            Rml::SetSystemInterface(&systemInterface);

            fileInterface.Initialise(application);
            Rml::SetFileInterface(&fileInterface);

            renderInterface.Initialise(application);
            Rml::SetRenderInterface(&renderInterface);

            return Rml::Initialise() ? Status::Success : Status::Fail;
        }

        void Shutdown() noexcept
        {
            Rml::Shutdown();
        }
        
        [[nodiscard]] Status LoadFontFace(const StringView& filepath, const bool isFallbackFace)
        {
            return Rml::LoadFontFace(filepath.data(), isFallbackFace)
                ? Status::Success
                : Status::Fail;
        }
    }
}