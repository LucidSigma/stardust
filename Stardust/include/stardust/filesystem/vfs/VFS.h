#pragma once
#ifndef STARDUST_VFS_H
#define STARDUST_VFS_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

namespace stardust
{
    namespace vfs
    {
        [[nodiscard]] extern bool Initialise(const char* argv0);
        extern void Quit();

        extern void AddToSearchPath(const StringView& directory);
        extern void AddToSearchPath(const Vector<StringView>& directories);

        extern void SetWriteDirectory(const StringView& writeDirectory);

        [[nodiscard]] extern String GetDirectorySeparator();

        [[nodiscard]] extern Vector<String> GetAllFilesInDirectory(const StringView& directory);
        [[nodiscard]] extern Vector<String> GetAllFilesInDirectoryRecursive(const StringView& directory);
        [[nodiscard]] extern Vector<String> GetAllFileNamesInDirectory(const StringView& directory);
        [[nodiscard]] extern Vector<String> GetAllFileNamesInDirectoryRecursive(const StringView& directory);

        [[nodiscard]] extern bool DoesFileExist(const StringView& filepath);
        [[nodiscard]] extern bool IsDirectory(const StringView& filepath);

        [[nodiscard]] extern Vector<ubyte> ReadFileData(const StringView& filepath);
        [[nodiscard]] extern String ReadFileString(const StringView& filepath);
        [[nodiscard]] extern Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data);
        [[nodiscard]] extern Status WriteToFile(const StringView& filepath, const String& data);
        [[nodiscard]] extern Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data);
        [[nodiscard]] extern Status AppendToFile(const StringView& filepath, const String& data);

        [[nodiscard]] extern usize GetFileSize(const StringView& filepath);
    }
}

#endif