#pragma once
#ifndef STARDUST_FILESYSTEM_H
#define STARDUST_FILESYSTEM_H

#include "stardust/types/Containers.h"
#include "stardust/types/Primitives.h"
#include "stardust/utility/error_handling/Result.h"
#include "stardust/utility/error_handling/Status.h"

#ifdef STARDUST_PLATFORM_WINDOWS
#undef CreateDirectory
#endif

namespace stardust
{
    namespace filesystem
    {
        enum class FileError
        {
            CannotOpenFile,
            InvalidData,
        };

        [[nodiscard]] extern auto InitialiseApplicationBaseDirectory() -> Status;
        [[nodiscard]] extern auto GetApplicationBaseDirectory() -> const String&;
        [[nodiscard]] extern auto InitialiseApplicationPreferenceDirectory(const StringView organisationName, const StringView applicationName) -> Status;
        [[nodiscard]] extern auto GetApplicationPreferenceDirectory() -> const String&;

        [[nodiscard]] extern auto GetAllFiles(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto GetAllFilesRecursive(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto IterateFiles(const String directory) -> Generator<const String>;
        [[nodiscard]] extern auto IterateFilesRecursive(const String directory) -> Generator<const String>;

        [[nodiscard]] extern auto GetAllFilepaths(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto GetAllFilepathsRecursive(const StringView directory) -> List<String>;
        [[nodiscard]] extern auto IterateFilepaths(const String directory) -> Generator<const String>;
        [[nodiscard]] extern auto IterateFilepathsRecursive(const String directory) -> Generator<const String>;

        [[nodiscard]] extern auto DoesPathExist(const StringView filepath) -> bool;
        [[nodiscard]] extern auto IsDirectory(const StringView filepath) -> bool;

        [[nodiscard]] extern auto HasStem(const StringView filename) -> bool;
        [[nodiscard]] extern auto GetStem(const StringView filename) -> String;
        [[nodiscard]] extern auto HasExtension(const StringView filename) -> bool;
        [[nodiscard]] extern auto GetExtension(const StringView filename) -> String;

        [[nodiscard]] extern auto CreateDirectory(const StringView path) -> Status;
        [[nodiscard]] extern auto GetDirectorySeparator() -> String;

        [[nodiscard]] extern auto ReadFile(const StringView filepath) -> Result<String, FileError>;
        [[nodiscard]] extern auto ReadFileLines(const StringView filepath) -> Result<List<String>, FileError>;
        [[nodiscard]] extern auto IterateFileLines(const String filepath) -> Generator<const String>;
        [[nodiscard]] extern auto ReadFileBytes(const StringView filepath) -> Result<List<ubyte>, FileError>;
        [[nodiscard]] extern auto ReadJSON(const StringView filepath) -> Result<JSON, FileError>;
        [[nodiscard]] extern auto ReadMessagePack(const StringView filepath) -> Result<JSON, FileError>;
        [[nodiscard]] extern auto ReadTOML(const StringView filepath) -> Result<TOML, FileError>;
        [[nodiscard]] extern auto ReadXML(const StringView filepath) -> Result<XML, FileError>;

        [[nodiscard]] extern auto WriteToFile(const StringView filepath, const String& data) -> Status;
        [[nodiscard]] extern auto WriteBytesToFile(const StringView filepath, const List<ubyte>& data) -> Status;
        [[nodiscard]] extern auto WriteJSONToFile(const StringView filepath, const JSON& data) -> Status;
        [[nodiscard]] extern auto WriteMessagePackToFile(const StringView filepath, const JSON& data) -> Status;
        [[nodiscard]] extern auto WriteTOMLToFile(const StringView filepath, const TOML& data) -> Status;
        [[nodiscard]] extern auto WriteXMLToFile(const StringView filepath, const XML& data) -> Status;

        [[nodiscard]] extern auto GetFileSize(const StringView filepath) -> usize;
    }

    namespace fs = filesystem;
}

#endif
