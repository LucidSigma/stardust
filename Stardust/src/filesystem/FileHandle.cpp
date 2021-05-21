#include "stardust/filesystem/FileHandle.h"

#include <utility>

namespace stardust
{
    namespace filesystem
    {
        FileHandle::FileHandle(const StringView& filepath, const OpenMode openMode, const OpenFormat openFormat)
        {
            Open(filepath, openMode, openFormat);
        }

        FileHandle::FileHandle(FileHandle&& other) noexcept
        {
            Close();

            m_handle = std::exchange(other.m_handle, nullptr);
        }

        FileHandle& FileHandle::operator=(FileHandle&& other) noexcept
        {
            Close();
            m_handle = std::exchange(other.m_handle, nullptr);

            return *this;
        }

        FileHandle::~FileHandle() noexcept
        {
            Close();
        }

        void FileHandle::Open(const StringView& filepath, const OpenMode openMode, const OpenFormat openFormat)
        {
            static const HashMap<OpenMode, String> openModeStringLookup{
                { OpenMode::Read, "r" },
                { OpenMode::Write, "w" },
                { OpenMode::Append, "a" },
                { OpenMode::ReadWrite, "r+" },
                { OpenMode::ReadWriteReplace, "w+" },
                { OpenMode::ReadAppend, "a+" },
            };

            String openString = openModeStringLookup.at(openMode);

            if (openFormat == OpenFormat::Binary)
            {
                openString += "b";
            }

            m_handle = SDL_RWFromFile(filepath.data(), openString.c_str());
        }

        void FileHandle::Close() noexcept
        {
            if (m_handle != nullptr)
            {
                SDL_RWclose(m_handle);
                m_handle = nullptr;
            }
        }
        
        [[nodiscard]] Optional<i64> FileHandle::Tell() const
        {
            const i64 tellResult = SDL_RWtell(m_handle);

            if (tellResult == -1)
            {
                return NullOpt;
            }
            else
            {
                return tellResult;
            }
        }

        [[nodiscard]] Optional<i64> FileHandle::Seek(const SeekLocation location, const i64 offset) const
        {
            const i64 seekResult = SDL_RWseek(m_handle, offset, static_cast<i32>(location));

            if (seekResult == -1)
            {
                return NullOpt;
            }
            else
            {
                return seekResult;
            }
        }

        [[nodiscard]] Vector<ubyte> FileHandle::Read(const usize byteCount) const
        {
            Vector<ubyte> readBytes{ };
            readBytes.resize(byteCount);

            const usize readResult = SDL_RWread(m_handle, readBytes.data(), byteCount, 1u);

            return readResult == 0u ? Vector<ubyte>{ } : readBytes;
        }

        [[nodiscard]] String FileHandle::ReadString(const usize byteCount) const
        {
            String readString;
            readString.resize(byteCount);

            const usize readResult = SDL_RWread(m_handle, readString.data(), byteCount, 1u);

            return readResult == 0u ? "" : readString;
        }

        [[nodiscard]] Status FileHandle::Write(const Vector<ubyte>& bytes) const
        {
            const usize writeResult = SDL_RWwrite(m_handle, bytes.data(), bytes.size(), 1u);

            return writeResult == 1u ? Status::Success : Status::Fail;
        }

        [[nodiscard]] Status FileHandle::Write(const String& string) const
        {
            const usize writeResult = SDL_RWwrite(m_handle, string.data(), string.size(), 1u);

            return writeResult == 1u ? Status::Success : Status::Fail;
        }

        [[nodiscard]] usize FileHandle::Size() const
        {
            const i64 sizeResult = SDL_RWsize(m_handle);

            if (sizeResult == -1)
            {
                return 0u;
            }
            else
            {
                return static_cast<usize>(sizeResult);
            }
        }
    }
}