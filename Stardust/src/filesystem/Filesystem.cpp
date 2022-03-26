#include "stardust/filesystem/Filesystem.h"

#include <filesystem>
#include <fstream>
#include <ios>
#include <iterator>
#include <sstream>
#include <string>
#include <system_error>
#include <utility>

#include <SDL2/SDL.h>

#include <pugixml/pugixml.hpp>
#include <toml++/toml.h>

namespace stardust
{
    namespace filesystem
    {
        namespace
        {
            String applicationBaseDirectory;
            String applicationPreferenceDirectory;
        }

        [[nodiscard]] auto InitialiseApplicationBaseDirectory() -> Status
        {
            if (applicationBaseDirectory.empty())
            {
                if (char* baseDirectoryPointer = SDL_GetBasePath();
                    baseDirectoryPointer != nullptr)
                {
                    applicationBaseDirectory = baseDirectoryPointer;

                    SDL_free(baseDirectoryPointer);
                    baseDirectoryPointer = nullptr;
                }
            }

            if (applicationBaseDirectory.empty())
            {
                return Status::Fail;
            }

            return Status::Success;
        }

        [[nodiscard]] auto GetApplicationBaseDirectory() -> const String&
        {
            return applicationBaseDirectory;
        }

        [[nodiscard]] auto InitialiseApplicationPreferenceDirectory(const StringView organisationName, const StringView applicationName) -> Status
        {
            if (applicationPreferenceDirectory.empty())
            {
                if (char* preferenceDirectoryPointer = SDL_GetPrefPath(organisationName.data(), applicationName.data());
                    preferenceDirectoryPointer != nullptr)
                {
                    applicationPreferenceDirectory = preferenceDirectoryPointer;

                    SDL_free(preferenceDirectoryPointer);
                    preferenceDirectoryPointer = nullptr;
                }
            }

            if (applicationPreferenceDirectory.empty())
            {
                return Status::Fail;
            }

            return Status::Success;
        }

        [[nodiscard]] auto GetApplicationPreferenceDirectory() -> const String&
        {
            return applicationPreferenceDirectory;
        }

        [[nodiscard]] auto GetAllFiles(const StringView directory) -> List<String>
        {
            List<String> files{ };

            for (const auto& file : std::filesystem::directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().filename().string());
                }
            }

            return files;
        }

        [[nodiscard]] auto GetAllFilesRecursive(const StringView directory) -> List<String>
        {
            List<String> files{ };

            for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().filename().string());
                }
            }

            return files;
        }

        [[nodiscard]] auto IterateFiles(const String directory) -> Generator<const String>
        {
            for (const auto& file : std::filesystem::directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    const String filename = file.path().filename().string();

                    co_yield filename;
                }
            }
        }

        [[nodiscard]] auto IterateFilesRecursive(const String directory) -> Generator<const String>
        {
            for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    const String filename = file.path().filename().string();

                    co_yield filename;
                }
            }
        }

        [[nodiscard]] auto GetAllFilepaths(const StringView directory) -> List<String>
        {
            List<String> files{ };

            for (const auto& file : std::filesystem::directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().string());
                }
            }

            return files;
        }

        [[nodiscard]] auto GetAllFilepathsRecursive(const StringView directory) -> List<String>
        {
            List<String> files{ };

            for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().string());
                }
            }

            return files;
        }

        [[nodiscard]] auto IterateFilepaths(const String directory) -> Generator<const String>
        {
            for (const auto& file : std::filesystem::directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    const String filename = file.path().string();

                    co_yield filename;
                }
            }
        }

        [[nodiscard]] auto IterateFilepathsRecursive(const String directory) -> Generator<const String>
        {
            for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    const String filename = file.path().string();

                    co_yield filename;
                }
            }
        }

        [[nodiscard]] auto DoesPathExist(const StringView filepath) -> bool
        {
            return std::filesystem::exists(filepath);
        }

        [[nodiscard]] auto IsDirectory(const StringView filepath) -> bool
        {
            return std::filesystem::is_directory(filepath);
        }

        [[nodiscard]] auto HasStem(const StringView filename) -> bool
        {
            return std::filesystem::path(filename).has_stem();
        }

        [[nodiscard]] auto GetStem(const StringView filename) -> String
        {
            return std::filesystem::path(filename).stem().string();
        }

        [[nodiscard]] auto HasExtension(const StringView filename) -> bool
        {
            return std::filesystem::path(filename).has_extension();
        }

        [[nodiscard]] auto GetExtension(const StringView filename) -> String
        {
            return std::filesystem::path(filename).extension().string();
        }

        [[nodiscard]] auto CreateDirectory(const StringView path) -> Status
        {
            return std::filesystem::create_directory(path)
                ? Status::Success
                : Status::Fail;
        }

        [[nodiscard]] auto GetDirectorySeparator() -> String
        {
        #ifdef STARDUST_PLATFORM_WINDOWS
            return "\\";
        #else
            return "/";
        #endif
        }

        [[nodiscard]] auto ReadFile(const StringView filepath) -> Result<String, FileError>
        {
            std::ifstream file(filepath.data());

            if (!file.is_open())
            {
                return Error<FileError>(FileError::CannotOpenFile);
            }

            std::ostringstream fileDataStream;
            fileDataStream << file.rdbuf();
            file.close();

            return Ok<String>(fileDataStream.str());
        }

        [[nodiscard]] auto ReadFileLines(const StringView filepath) -> Result<List<String>, FileError>
        {
            std::ifstream file(filepath.data());

            if (!file.is_open())
            {
                return Error<FileError>(FileError::CannotOpenFile);
            }

            List<String> fileLines{ };
            String currentLine;

            while (std::getline(file, currentLine))
            {
                fileLines.push_back(currentLine);
            }

            return Ok<List<String>>(std::move(fileLines));
        }

        [[nodiscard]] auto IterateFileLines(const String filepath) -> Generator<const String>
        {
            std::ifstream file(filepath);

            if (file.is_open())
            {
                String currentLine;

                while (std::getline(file, currentLine))
                {
                    co_yield currentLine;
                }
            }
        }

        [[nodiscard]] auto ReadFileBytes(const StringView filepath) -> Result<List<ubyte>, FileError>
        {
            std::basic_ifstream<ubyte> file(filepath.data(), std::ios::in | std::ios::binary);

            if (!file.is_open())
            {
                return Error<FileError>(FileError::CannotOpenFile);
            }

            return Ok<List<ubyte>>(
                List<ubyte>(
                    std::istreambuf_iterator<ubyte>(file),
                    std::istreambuf_iterator<ubyte>()
                )
            );
        }

        [[nodiscard]] auto ReadJSON(const StringView filepath) -> Result<JSON, FileError>
        {
            const Result<String, FileError> fileData = ReadFile(filepath);

            if (fileData.is_err())
            {
                return Error<FileError>(FileError::CannotOpenFile);
            }

            JSON jsonData = JSON::parse(fileData.value(), nullptr, false);

            if (jsonData.is_discarded())
            {
                return Error<FileError>(FileError::InvalidData);
            }

            return Ok<JSON>(std::move(jsonData));
        }

        [[nodiscard]] auto ReadMessagePack(const StringView filepath) -> Result<JSON, FileError>
        {
            auto fileReadResult = ReadFileBytes(filepath);

            if (fileReadResult.is_err())
            {
                return Error<FileError>(std::move(fileReadResult).unwrap_err());
            }

            const auto fileData = std::move(fileReadResult).unwrap();
            JSON messagePackAsJSON = JSON::from_msgpack(fileData, false, false);

            if (messagePackAsJSON.is_discarded())
            {
                return Error<FileError>(FileError::InvalidData);
            }

            return Ok<JSON>(std::move(messagePackAsJSON));
        }

        [[nodiscard]] auto ReadTOML(const StringView filepath) -> Result<TOML, FileError>
        {
            TOML tomlData{ };

            if (!DoesPathExist(filepath))
            {
                return Error<FileError>(FileError::CannotOpenFile);
            }

            try
            {
                tomlData = toml::parse_file(filepath);
            }
            catch (const toml::parse_error&)
            {
                return Error<FileError>(FileError::InvalidData);
            }

            return Ok<TOML>(std::move(tomlData));
        }

        [[nodiscard]] auto ReadXML(const StringView filepath) -> Result<XML, FileError>
        {
            XML xmlDocument;
            const pugi::xml_parse_result parseResult = xmlDocument.load_file(filepath.data());

            switch (parseResult.status)
            {
            case pugi::status_ok:
                return Ok<XML>(std::move(xmlDocument));

            case pugi::status_file_not_found:
            case pugi::status_io_error:
            case pugi::status_out_of_memory:
            case pugi::status_internal_error:
                return Error<FileError>(FileError::CannotOpenFile);

            default:
                return Error<FileError>(FileError::InvalidData);
            }
        }

        [[nodiscard]] auto WriteToFile(const StringView filepath, const String& data) -> Status
        {
            std::ofstream outputFile(filepath.data(), std::ios::out | std::ios::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] auto WriteBytesToFile(const StringView filepath, const List<ubyte>& data) -> Status
        {
            std::ofstream outputFile(filepath.data(), std::ios::out | std::ios::binary | std::ios::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile.write(reinterpret_cast<const char*>(data.data()), static_cast<std::streamsize>(data.size()));
            outputFile << data.data();
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] auto WriteJSONToFile(const StringView filepath, const JSON& data) -> Status
        {
            std::ofstream outputFile(filepath.data(), std::ios::out | std::ios::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] auto WriteMessagePackToFile(const StringView filepath, const JSON& data) -> Status
        {
            const List<ubyte> messagePackData = JSON::to_msgpack(data);

            if (messagePackData.empty())
            {
                return Status::Fail;
            }

            return WriteBytesToFile(filepath, messagePackData);
        }

        [[nodiscard]] auto WriteTOMLToFile(const StringView filepath, const TOML& data) -> Status
        {
            std::ofstream outputFile(filepath.data(), std::ios::out | std::ios::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] auto WriteXMLToFile(const StringView filepath, const XML& data) -> Status
        {
            const bool didSaveSuccessfully = data.save_file(filepath.data());

            return didSaveSuccessfully ? Status::Success : Status::Fail;
        }

        [[nodiscard]] auto GetFileSize(const StringView filepath) -> usize
        {
            std::error_code errorCode{ };
            const umax fileSize = std::filesystem::file_size(filepath, errorCode);

            return fileSize != std::numeric_limits<umax>::max() ? static_cast<usize>(fileSize) : 0u;
        }
    }
}