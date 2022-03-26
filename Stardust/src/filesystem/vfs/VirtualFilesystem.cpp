#include "stardust/filesystem/vfs/VirtualFilesystem.h"

#include <format>
#include <utility>

#include <physfs/physfs.h>
#include <toml++/toml.h>

namespace stardust
{
    namespace vfs
    {
        [[nodiscard]] auto Initialise(const char* argv0) -> Status
        {
            return PHYSFS_init(argv0) != 0
                ? Status::Success
                : Status::Fail;
        }

        auto Quit() -> void
        {
            if (PHYSFS_isInit() != 0)
            {
                PHYSFS_deinit();
            }
        }

        [[nodiscard]] auto AddToSearchPath(const StringView directory) -> Status
        {
            return PHYSFS_mount(directory.data(), nullptr, 1) != 0
                ? Status::Success
                : Status::Fail;
        }

        [[nodiscard]] auto GetAllFiles(const StringView directory) -> List<String>
        {
            List<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

            if (enumeratedFiles == nullptr)
            {
                return { };
            }

            for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
            {
                if (!IsDirectory(std::format("{}/{}", directory, *filename)))
                {
                    files.push_back(String(*filename));
                }
            }

            PHYSFS_freeList(enumeratedFiles);
            enumeratedFiles = nullptr;

            return files;
        }

        [[nodiscard]] auto GetAllFilesRecursive(const StringView directory) -> List<String>
        {
            List<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

            if (enumeratedFiles == nullptr)
            {
                return { };
            }

            for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
            {
                const String fullFilepath = std::format("{}/{}", directory, *filename);

                if (!IsDirectory(fullFilepath))
                {
                    files.push_back(String(*filename));
                }
                else
                {
                    const auto filesInDirectory = GetAllFilesRecursive(fullFilepath);
                    files.insert(std::cend(files), std::cbegin(filesInDirectory), std::cend(filesInDirectory));
                }
            }

            PHYSFS_freeList(enumeratedFiles);
            enumeratedFiles = nullptr;

            return files;
        }

        [[nodiscard]] auto IterateFiles(const String directory) -> Generator<const String>
        {
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.c_str());

            if (enumeratedFiles != nullptr)
            {
                for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
                {
                    if (!IsDirectory(std::format("{}/{}", directory, *filename)))
                    {
                        co_yield String(*filename);
                    }
                }

                PHYSFS_freeList(enumeratedFiles);
                enumeratedFiles = nullptr;
            }
        }

        [[nodiscard]] auto IterateFilesRecursive(String directory) -> Generator<const String>
        {
            Queue<String> directoriesToEnumerate{ };
            directoriesToEnumerate.push(std::move(directory));

            while (!directoriesToEnumerate.empty())
            {
                char** enumeratedFiles = PHYSFS_enumerateFiles(directoriesToEnumerate.front().c_str());

                if (enumeratedFiles != nullptr)
                {
                    for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
                    {
                        const String fullFilepath = std::format("{}/{}", directoriesToEnumerate.front().c_str(), *filename);

                        if (!IsDirectory(fullFilepath))
                        {
                            co_yield String(*filename);
                        }
                        else
                        {
                            directoriesToEnumerate.push(fullFilepath);
                        }
                    }

                    PHYSFS_freeList(enumeratedFiles);
                    enumeratedFiles = nullptr;
                }

                directoriesToEnumerate.pop();
            }
        }

        [[nodiscard]] auto GetAllFilepaths(const StringView directory) -> List<String>
        {
            List<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

            if (enumeratedFiles == nullptr)
            {
                return { };
            }

            for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
            {
                if (const String fullFilepath = std::format("{}/{}", directory, *filename);
                    !IsDirectory(fullFilepath))
                {
                    files.push_back(fullFilepath);
                }
            }

            PHYSFS_freeList(enumeratedFiles);
            enumeratedFiles = nullptr;

            return files;
        }

        [[nodiscard]] auto GetAllFilepathsRecursive(const StringView directory) -> List<String>
        {
            List<String> files{ };
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.data());

            if (enumeratedFiles == nullptr)
            {
                return { };
            }

            for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
            {
                const String fullFilepath = std::format("{}/{}", directory, *filename);

                if (!IsDirectory(fullFilepath))
                {
                    files.push_back(fullFilepath);
                }
                else
                {
                    const auto filesInDirectory = GetAllFilesRecursive(fullFilepath);
                    files.insert(std::cend(files), std::cbegin(filesInDirectory), std::cend(filesInDirectory));
                }
            }

            PHYSFS_freeList(enumeratedFiles);
            enumeratedFiles = nullptr;

            return files;
        }

        [[nodiscard]] auto IterateFilepaths(const String directory) -> Generator<const String>
        {
            char** enumeratedFiles = PHYSFS_enumerateFiles(directory.c_str());

            if (enumeratedFiles != nullptr)
            {
                for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
                {
                    if (const String fullFilepath = std::format("{}/{}", directory, *filename);
                        !IsDirectory(fullFilepath))
                    {
                        co_yield fullFilepath;
                    }
                }

                PHYSFS_freeList(enumeratedFiles);
                enumeratedFiles = nullptr;
            }
        }

        [[nodiscard]] auto IterateFilepathsRecursive(String directory) -> Generator<const String>
        {
            Queue<String> directoriesToEnumerate{ };
            directoriesToEnumerate.push(std::move(directory));

            while (!directoriesToEnumerate.empty())
            {
                char** enumeratedFiles = PHYSFS_enumerateFiles(directoriesToEnumerate.front().c_str());

                if (enumeratedFiles != nullptr)
                {
                    for (const char* const* filename = enumeratedFiles; *filename != nullptr; ++filename)
                    {
                        const String fullFilepath = std::format("{}/{}", directoriesToEnumerate.front().c_str(), *filename);

                        if (!IsDirectory(fullFilepath))
                        {
                            co_yield fullFilepath;
                        }
                        else
                        {
                            directoriesToEnumerate.push(fullFilepath);
                        }
                    }

                    PHYSFS_freeList(enumeratedFiles);
                    enumeratedFiles = nullptr;
                }

                directoriesToEnumerate.pop();
            }
        }

        [[nodiscard]] auto DoesPathExist(const StringView filepath) -> bool
        {
            return PHYSFS_exists(filepath.data()) != 0;
        }
        
        [[nodiscard]] auto IsDirectory(const StringView filepath) -> bool
        {
            PHYSFS_Stat fileStats{ };

            if (PHYSFS_stat(filepath.data(), &fileStats) == 0)
            {
                return false;
            }

            return fileStats.filetype == PHYSFS_FileType::PHYSFS_FILETYPE_DIRECTORY;
        }

        [[nodiscard]] auto ReadFileBytes(const StringView filepath) -> Result<List<ubyte>, VirtualFileError>
        {
            PHYSFS_File* file = PHYSFS_openRead(filepath.data());

            if (file == nullptr)
            {
                return Error<VirtualFileError>(VirtualFileError::CannotOpenFile);
            }

            const usize fileSize = GetFileSize(filepath);

            if (fileSize == 0u)
            {
                return Error<VirtualFileError>(VirtualFileError::CannotReadFile);
            }

            List<ubyte> fileBytes(fileSize);

            if (PHYSFS_readBytes(file, fileBytes.data(), fileSize) == -1ll)
            {
                PHYSFS_close(file);
                file = nullptr;

                return Error<VirtualFileError>(VirtualFileError::CannotReadFile);
            }

            PHYSFS_close(file);
            file = nullptr;

            return Ok<List<ubyte>>(std::move(fileBytes));
        }

        [[nodiscard]] auto ReadFileString(const StringView filepath) -> Result<String, VirtualFileError>
        {
            PHYSFS_File* file = PHYSFS_openRead(filepath.data());

            if (file == nullptr)
            {
                return Error<VirtualFileError>(VirtualFileError::CannotOpenFile);
            }

            const usize fileSize = GetFileSize(filepath);

            if (fileSize == 0u)
            {
                return Error<VirtualFileError>(VirtualFileError::CannotReadFile);
            }

            String fileString(fileSize, '\0');

            if (PHYSFS_readBytes(file, fileString.data(), fileSize) == -1ll)
            {
                PHYSFS_close(file);
                file = nullptr;

                return Error<VirtualFileError>(VirtualFileError::CannotReadFile);
            }

            PHYSFS_close(file);
            file = nullptr;

            return Ok<String>(std::move(fileString));
        }

        [[nodiscard]] auto ReadJSON(const StringView filepath) -> Result<JSON, VirtualFileError>
        {
            auto fileDataResult = ReadFileBytes(filepath);

            if (fileDataResult.is_err())
            {
                return Error<VirtualFileError>(std::move(fileDataResult).unwrap_err());
            }

            List<ubyte> fileData = std::move(fileDataResult).unwrap();

            JSON jsonData = JSON::parse(
                reinterpret_cast<const u8*>(fileData.data()),
                reinterpret_cast<const u8*>(fileData.data()) + fileData.size(),
                nullptr,
                false
            );

            if (jsonData.is_discarded())
            {
                return Error<VirtualFileError>(VirtualFileError::InvalidData);
            }

            return Ok<JSON>(std::move(jsonData));
        }

        [[nodiscard]] auto ReadMessagePack(const StringView filepath) -> Result<JSON, VirtualFileError>
        {
            auto fileReadResult = ReadFileBytes(filepath);

            if (fileReadResult.is_err())
            {
                return Error<VirtualFileError>(std::move(fileReadResult).unwrap_err());
            }

            JSON messagePackAsJSON = JSON::from_msgpack(std::move(fileReadResult).unwrap(), false, false);

            if (messagePackAsJSON.is_discarded())
            {
                return Error<VirtualFileError>(VirtualFileError::InvalidData);
            }

            return Ok<JSON>(std::move(messagePackAsJSON));
        }

        [[nodiscard]] auto ReadTOML(const StringView filepath) -> Result<TOML, VirtualFileError>
        {
            try
            {
                auto tomlStringResult = ReadFileString(filepath);

                if (tomlStringResult.is_err())
                {
                    return Error<VirtualFileError>(std::move(tomlStringResult).unwrap_err());
                }

                const String tomlString = std::move(tomlStringResult).unwrap();
                TOML tomlData = toml::parse(tomlString);

                return Ok<TOML>(std::move(tomlData));
            }
            catch (const toml::parse_error&)
            {
                return Error<VirtualFileError>(VirtualFileError::InvalidData);
            }
        }

        [[nodiscard]] auto ReadXML(const StringView filepath) -> Result<XML, VirtualFileError>
        {
            auto fileReadResult = ReadFileBytes(filepath);

            if (fileReadResult.is_err())
            {
                return Error<VirtualFileError>(std::move(fileReadResult).unwrap_err());
            }

            const List<ubyte> xmlBuffer = std::move(fileReadResult).unwrap();

            XML xmlDocument;
            const pugi::xml_parse_result parseResult = xmlDocument.load_buffer(xmlBuffer.data(), xmlBuffer.size());

            switch (parseResult.status)
            {
            case pugi::status_ok:
                return Ok<XML>(std::move(xmlDocument));

            case pugi::status_file_not_found:
            case pugi::status_out_of_memory:
                return Error<VirtualFileError>(VirtualFileError::CannotOpenFile);

            case pugi::status_io_error:
            case pugi::status_internal_error:
            case pugi::status_no_document_element:
                return Error<VirtualFileError>(VirtualFileError::CannotReadFile);

            default:
                return Error<VirtualFileError>(VirtualFileError::InvalidData);
            }
        }

        [[nodiscard]] auto GetFileSize(const StringView filepath) -> usize
        {
            PHYSFS_Stat fileStats{ };

            if (PHYSFS_stat(filepath.data(), &fileStats) == 0)
            {
                return 0u;
            }

            if (fileStats.filetype == PHYSFS_FileType::PHYSFS_FILETYPE_DIRECTORY)
            {
                return 0u;
            }

            return static_cast<usize>(fileStats.filesize);
        }
    }
}
