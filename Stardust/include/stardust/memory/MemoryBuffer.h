#pragma once
#ifndef STARDUST_MEMORY_BUFFER_H
#define STARDUST_MEMORY_BUFFER_H

#include "stardust/data/Containers.h"
#include "stardust/data/Pointers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    class MemoryBuffer
    {
    private:
        Vector<ubyte> m_buffer{ };
        ObserverPtr<ubyte> m_rwPointer = nullptr;

    public:
        explicit MemoryBuffer(const usize size = 0u);
        ~MemoryBuffer() noexcept = default;

        [[nodiscard]] inline bool IsPointerValid() const noexcept { return m_rwPointer != nullptr; }

        void SetPointer(const usize index);
        void ShiftPointer(const isize offset);
        void MovePointerToBeginning();
        void MovePointerToEnd();

        [[nodiscard]] inline ObserverPtr<ubyte> GetPointer() noexcept { return m_rwPointer; }
        [[nodiscard]] inline ObserverPtr<const ubyte> GetPointer() const noexcept { return m_rwPointer; }
        [[nodiscard]] usize GetPointerOffset() const;

        [[nodiscard]] Vector<ubyte> ReadBytes(const usize byteCount);
        void WriteBytes(const Vector<ubyte>& bytes);

        void Resize(const usize newSize);
        [[nodiscard]] inline usize GetSize() const noexcept { return m_buffer.size(); }
    };
}

#endif