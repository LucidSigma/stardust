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

        void VirtualFileHandle::Close() noexcept
        {
            if (m_handle != nullptr)
            {
                PHYSFS_close(m_handle);
                m_handle = nullptr;
            }
        }
    }
}