#pragma once
#ifndef STARDUST_VIRTUAL_FILESYSTEM_H
#define STARDUST_VIRTUAL_FILESYSTEM_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Status.h"
#include "stardust/utility/error_handling/Result.h"

namespace stardust
{
    namespace vfs
    {
        enum class VirtualFileError
        {
            CannotOpenFile,
            CannotReadFile,
            InvalidData,
        };

        [[nodiscard]] extern auto Initialise(const char* argv0) -> Status;
        extern auto Quit() -> void;

        [[nodiscard]] extern auto AddToSearchPath(const StringView directory) -> Status;

        [[nodiscard]] extern auto GetAllFiles(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto GetAllFilesRecursive(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto IterateFiles(const String directory) -> Generator<const String>;
        [[nodiscard]] extern auto IterateFilesRecursive(String directory) -> Generator<const String>;

        [[nodiscard]] extern auto GetAllFilepaths(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto GetAllFilepathsRecursive(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto IterateFilepaths(const String directory) -> Generator<const String>;
        [[nodiscard]] extern auto IterateFilepathsRecursive(String directory) -> Generator<const String>;

        [[nodiscard]] extern auto DoesPathExist(const StringView filepath) -> bool;
        [[nodiscard]] extern auto IsDirectory(const StringView filepath) -> bool;

        [[nodiscard]] extern auto ReadFileBytes(const StringView filepath) -> Result<List<ubyte>, VirtualFileError>;
        [[nodiscard]] extern auto ReadFileString(const StringView filepath) -> Result<String, VirtualFileError>;
        [[nodiscard]] extern auto ReadJSON(const StringView filepath) -> Result<JSON, VirtualFileError>;
        [[nodiscard]] extern auto ReadMessagePack(const StringView filepath) -> Result<JSON, VirtualFileError>;
        [[nodiscard]] extern auto ReadTOML(const StringView filepath) -> Result<TOML, VirtualFileError>;
        [[nodiscard]] extern auto ReadXML(const StringView filepath) -> Result<XML, VirtualFileError>;

        [[nodiscard]] extern auto GetFileSize(const StringView filepath) -> usize;
    }
}

#endif
