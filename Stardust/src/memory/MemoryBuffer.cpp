#include "stardust/memory/MemoryBuffer.h"

#include <cstring>

namespace stardust
{
    MemoryBuffer::MemoryBuffer(const usize size)
    {
        Resize(size);
    }

    void MemoryBuffer::SetPointer(const usize index)
    {
        m_rwPointer = m_rwPointer + index;
    }

    void MemoryBuffer::ShiftPointer(const isize offset)
    {
        m_rwPointer += offset;
    }

    void MemoryBuffer::MovePointerToBeginning()
    {
        m_rwPointer = m_buffer.data();
    }

    void MemoryBuffer::MovePointerToEnd()
    {
        m_rwPointer = m_buffer.data() + (m_buffer.size() - 1u);
    }

    [[nodiscard]] usize MemoryBuffer::GetPointerOffset() const
    {
        return static_cast<usize>(m_rwPointer - m_buffer.data());
    }

    [[nodiscard]] Vector<ubyte> MemoryBuffer::ReadBytes(const usize byteCount) const
    {
        Vector<ubyte> bytes(byteCount);
        std::memcpy(bytes.data(), m_rwPointer, byteCount);

        return bytes;
    }

    void MemoryBuffer::WriteBytes(const Vector<ubyte>& bytes)
    {
        std::memcpy(m_rwPointer, bytes.data(), bytes.size());
    }

    void MemoryBuffer::Resize(const usize newSize)
    {
        m_buffer.resize(newSize, 0u);
        m_rwPointer = m_buffer.data();
    }
}