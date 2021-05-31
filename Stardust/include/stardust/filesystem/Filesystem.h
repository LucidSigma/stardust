#pragma once
#ifndef STARDUST_FILESYSTEM_H
#define STARDUST_FILESYSTEM_H

#include <nlohmann/json.hpp>
#include <tinyxml2/tinyxml2.h>

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"
#include "stardust/utility/status/Status.h"

#undef CreateDirectory

namespace stardust
{
    namespace filesystem
    {
        [[nodiscard]] extern String GetApplicationBaseDirectory();
        [[nodiscard]] extern String GetApplicationPreferenceDirectory(const StringView& organisationName, const StringView& applicationName);

        [[nodiscard]] extern Vector<String> GetAllFilesInDirectory(const StringView& directory);
        [[nodiscard]] extern Vector<String> GetAllFilesInDirectoryRecursive(const StringView& directory);
        [[nodiscard]] extern Vector<String> GetAllFileNamesInDirectory(const StringView& directory);
        [[nodiscard]] extern Vector<String> GetAllFileNamesInDirectoryRecursive(const StringView& directory);

        [[nodiscard]] extern bool DoesFileExist(const StringView& filepath);
        [[nodiscard]] extern bool IsDirectory(const StringView& filepath);

        [[nodiscard]] extern String GetParentFilepath(const StringView& filepath);
        [[nodiscard]] extern String GetFilenameFromDirectory(const StringView& filepath);
        [[nodiscard]] extern String GetFileStem(const StringView& filename);
        [[nodiscard]] extern String GetFileExtension(const StringView& filename);

        [[nodiscard]] extern Status CreateDirectory(const StringView& path);

        [[nodiscard]] extern String ReadFile(const StringView& filepath);
        [[nodiscard]] extern Vector<String> ReadFileLines(const StringView& filepath);
        [[nodiscard]] extern Vector<ubyte> ReadFileBytes(const StringView& filepath);

        [[nodiscard]] extern Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data);
        [[nodiscard]] extern Status WriteToFile(const StringView& filepath, const String& data);
        [[nodiscard]] extern Status WriteToFile(const StringView& filepath, const nlohmann::json& data);
        [[nodiscard]] extern Status WriteToFile(const StringView& filepath, const tinyxml2::XMLDocument& data);

        [[nodiscard]] extern Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data);
        [[nodiscard]] extern Status AppendToFile(const StringView& filepath, const String& data);
        [[nodiscard]] extern Status AppendToFile(const StringView& filepath, const nlohmann::json& data);
        [[nodiscard]] extern Status AppendToFile(const StringView& filepath, const tinyxml2::XMLDocument& data);

        [[nodiscard]] extern usize GetFileSize(const StringView& filepath);

        [[nodiscard]] extern nlohmann::json ReadJSON(const StringView& filepath);
        [[nodiscard]] extern Status ReadXML(const StringView& filepath, tinyxml2::XMLDocument& document);

        [[nodiscard]] extern Status SaveToMessagePack(const StringView& filepath, const nlohmann::json& data);
        [[nodiscard]] extern nlohmann::json ReadMessagePack(const StringView& filepath);
    }

    namespace fs = filesystem;
}

#endif