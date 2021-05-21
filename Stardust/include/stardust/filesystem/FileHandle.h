#pragma once
#ifndef STARDUST_FILE_HANDLE_H
#define STARDUST_FILE_HANDLE_H

#include "stardust/utility/interfaces/INoncopyable.h"

#include <SDL2/SDL.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    namespace filesystem
    {
        class FileHandle
            : private INoncopyable
        {
        public:
            enum class OpenMode
            {
                Read,
                Write,
                Append,
                ReadWrite,
                ReadAppend,
                ReadWriteReplace,
            };

            enum class OpenFormat
            {
                Text,
                Binary,
            };

            enum class SeekLocation
                : decltype(RW_SEEK_SET)
            {
                Beginning = RW_SEEK_SET,
                Current = RW_SEEK_CUR,
                End = RW_SEEK_END,
            };

        private:
            SDL_RWops* m_handle = nullptr;

        public:
            FileHandle() = default;
            FileHandle(const StringView& filepath, const OpenMode openMode, const OpenFormat openFormat);

            FileHandle(FileHandle&& other) noexcept;
            FileHandle& operator =(FileHandle&& other) noexcept;

            ~FileHandle() noexcept;

            void Open(const StringView& filepath, const OpenMode openMode, const OpenFormat openFormat);
            void Close() noexcept;

            [[nodiscard]] inline bool IsValid() const noexcept { return m_handle != nullptr; }

            [[nodiscard]] Optional<i64> Tell() const;
            [[nodiscard]] Optional<i64> Seek(const SeekLocation location, const i64 offset) const;

            [[nodiscard]] Vector<ubyte> Read(const usize bytes) const;
            [[nodiscard]] Status Write(const String& string) const;
            [[nodiscard]] Status Write(const Vector<ubyte>& bytes) const;

            [[nodiscard]] usize Size() const;
        };
    }
}

#endif