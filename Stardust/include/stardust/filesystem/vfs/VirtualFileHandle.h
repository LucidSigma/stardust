#pragma once
#ifndef STARDUST_VIRTUAL_FILE_HANDLE_H
#define STARDUST_VIRTUAL_FILE_HANDLE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <physfs/physfs.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    namespace vfs
    {
        class VirtualFileHandle
            : private INoncopyable
        {
        public:
            enum class OpenMode
            {
                Read,
                Write,
                Append,
            };

        private:
            PHYSFS_File* m_handle = nullptr;

        public:
            VirtualFileHandle() = default;
            VirtualFileHandle(const StringView& filepath, const OpenMode openMode);

            VirtualFileHandle(VirtualFileHandle&& other) noexcept;
            VirtualFileHandle& operator =(VirtualFileHandle&& other) noexcept;

            ~VirtualFileHandle() noexcept;

            void Open(const StringView& filepath, const OpenMode openMode);
            void Close() noexcept;

            [[nodiscard]] inline bool IsValid() const noexcept { return m_handle != nullptr; }

            [[nodiscard]] Optional<i64> Tell() const;
            Status Seek(const u64 offset) const;
            [[nodiscard]] bool IsAtEOF() const;

            [[nodiscard]] Vector<ubyte> Read(const usize byteCount) const;
            [[nodiscard]] Status Write(const Vector<ubyte>& bytes) const;
            [[nodiscard]] Status Write(const String& string) const;

            [[nodiscard]] usize Size() const;

            [[nodiscard]] PHYSFS_File* GetRawHandle() const noexcept { return m_handle; }
        };
    }
}

#endif