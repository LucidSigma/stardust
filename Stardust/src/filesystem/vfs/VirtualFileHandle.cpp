#include "stardust/filesystem/vfs/VirtualFileHandle.h"

#include <utility>

namespace stardust
{
    namespace vfs
    {
        VirtualFileHandle::VirtualFileHandle(const StringView& filepath, const OpenMode openMode)
        {
            Open(filepath, openMode);
        }

        VirtualFileHandle::VirtualFileHandle(VirtualFileHandle&& other) noexcept
        {
            Close();
            m_handle = std::exchange(other.m_handle, nullptr);
        }

        VirtualFileHandle& VirtualFileHandle::operator =(VirtualFileHandle&& other) noexcept
        {
            Close();
            m_handle = std::exchange(other.m_handle, nullptr);

            return *this;
        }

        VirtualFileHandle::~VirtualFileHandle() noexcept
        {
            Close();
        }

        void VirtualFileHandle::Open(const StringView& filepath, const OpenMode openMode)
        {
            switch (openMode)
            {
            case OpenMode::Read:
                m_handle = PHYSFS_openRead(filepath.data());

                break;

            case OpenMode::Write:
                m_handle = PHYSFS_openWrite(filepath.data());

                break;

            case OpenMode::Append:
                m_handle = PHYSFS_openAppend(filepath.data());

                break;

            default:
                break;
            }
        }

        void VirtualFileHandle::Close() noexcept
        {
            if (m_handle != nullptr)
            {
                PHYSFS_close(m_handle);
                m_handle = nullptr;
            }
        }

        [[nodiscard]] Optional<i64> VirtualFileHandle::Tell() const
        {
            const PHYSFS_sint64 tellResult = PHYSFS_tell(m_handle);

            if (tellResult == -1)
            {
                return NullOpt;
            }
            else
            {
                return static_cast<i64>(tellResult);
            }
        }

        [[nodiscard]] Status VirtualFileHandle::Seek(const u64 offset) const
        {
            const i32 seekResult = PHYSFS_seek(m_handle, offset);

            return seekResult == 0 ? Status::Fail : Status::Success;
        }

        [[nodiscard]] bool VirtualFileHandle::IsAtEOF() const
        {
            return PHYSFS_eof(m_handle) != 0;
        }

        [[nodiscard]] Vector<ubyte> VirtualFileHandle::Read(const usize byteCount) const
        {
            const PHYSFS_sint64 fileSize = PHYSFS_fileLength(m_handle);
            Vector<ubyte> fileData(fileSize);

            const PHYSFS_sint64 readResult =  PHYSFS_readBytes(m_handle, fileData.data(), fileSize);

            return readResult == -1ll ? Vector<ubyte>{ } : fileData;
        }

        [[nodiscard]] Status VirtualFileHandle::Write(const Vector<ubyte>& bytes) const
        {
            const PHYSFS_sint64 writeResult = PHYSFS_writeBytes(m_handle, bytes.data(), bytes.size());
            
            return writeResult == -1ll ? Status::Fail : Status::Success;
        }

        [[nodiscard]] Status VirtualFileHandle::Write(const String& string) const
        {
            const PHYSFS_sint64 writeResult = PHYSFS_writeBytes(m_handle, string.data(), string.size());
            
            return writeResult == -1ll ? Status::Fail : Status::Success;
        }

        void VirtualFileHandle::Flush() const
        {
            PHYSFS_flush(m_handle);
        }
        
        [[nodiscard]] usize VirtualFileHandle::Size() const
        {
            return static_cast<usize>(PHYSFS_fileLength(m_handle));
        }
    }
}