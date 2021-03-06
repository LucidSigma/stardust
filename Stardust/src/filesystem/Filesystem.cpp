#include "stardust/filesystem/Filesystem.h"

#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>
#include <system_error>

#include <SDL2/SDL.h>

namespace stardust
{
    namespace filesystem
    {
        namespace
        {
            String applicationBaseDirectory;
            String applicationPreferenceDirectory;
        }

        [[nodiscard]] Status InitialiseApplicationBaseDirectory()
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

        [[nodiscard]] Status InitialiseApplicationPreferenceDirectory(const StringView& organisationName, const StringView& applicationName)
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

        [[nodiscard]] const String& GetApplicationBaseDirectory()
        {
            return applicationBaseDirectory;
        }

        [[nodiscard]] const String& GetApplicationPreferenceDirectory()
        {
            return applicationPreferenceDirectory;
        }

        [[nodiscard]] Vector<String> GetAllFilesInDirectory(const StringView& directory)
        {
            Vector<String> files{ };

            for (const auto& file : std::filesystem::directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().string());
                }
            }

            return files;
        }

        [[nodiscard]] Vector<String> GetAllFilesInDirectoryRecursive(const StringView& directory)
        {
            Vector<String> files{ };

            for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().string());
                }
            }

            return files;
        }

        [[nodiscard]] Vector<String> GetAllFileNamesInDirectory(const StringView& directory)
        {
            Vector<String> files{ };

            for (const auto& file : std::filesystem::directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().filename().string());
                }
            }

            return files;
        }

        [[nodiscard]] Vector<String> GetAllFileNamesInDirectoryRecursive(const StringView& directory)
        {
            Vector<String> files{ };

            for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
            {
                if (!file.is_directory())
                {
                    files.push_back(file.path().filename().string());
                }
            }

            return files;
        }

        [[nodiscard]] bool DoesFileExist(const StringView& filepath)
        {
            return std::filesystem::exists(filepath);
        }

        [[nodiscard]] bool IsDirectory(const StringView& filepath)
        {
            return std::filesystem::is_directory(filepath);
        }

        [[nodiscard]] String GetParentFilepath(const StringView& filepath)
        {
            return std::filesystem::path(filepath).parent_path().string();
        }

        [[nodiscard]] String GetFilenameFromDirectory(const StringView& filepath)
        {
            return std::filesystem::path(filepath).filename().string();
        }

        [[nodiscard]] String GetFileStem(const StringView& filename)
        {
            return std::filesystem::path(filename).stem().string();
        }

        [[nodiscard]] String GetFileExtension(const StringView& filename)
        {
            return std::filesystem::path(filename).extension().string();
        }

        [[nodiscard]] Status CreateDirectory(const StringView& path)
        {
            return std::filesystem::create_directory(path)
                ? Status::Success
                : Status::Fail;
        }

        [[nodiscard]] String ReadFile(const StringView& filepath)
        {
            std::ifstream file(filepath);

            if (!file.is_open())
            {
                return "";
            }

            std::ostringstream fileDataStream;
            fileDataStream << file.rdbuf();
            file.close();

            return fileDataStream.str();
        }

        [[nodiscard]] Vector<String> ReadFileLines(const StringView& filepath)
        {
            std::ifstream file(filepath);

            if (!file.is_open())
            {
                return { };
            }

            Vector<String> fileLines{ };
            String currentLine;

            while (std::getline(file, currentLine))
            {
                fileLines.push_back(currentLine);
            }

            return fileLines;
        }

        [[nodiscard]] Vector<ubyte> ReadFileBytes(const StringView& filepath)
        {
            std::basic_ifstream<ubyte> file(filepath, std::ios_base::in | std::ios_base::binary);

            if (!file.is_open())
            {
                return { };
            }

            return Vector<ubyte>(
                std::istreambuf_iterator<ubyte>(file),
                std::istreambuf_iterator<ubyte>()
            );
        }

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const Vector<ubyte>& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data.data();
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const String& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const nlohmann::json& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const tinyxml2::XMLDocument& data)
        {
            tinyxml2::XMLPrinter xmlPrinter;
            data.Print(&xmlPrinter);

            return WriteToFile(filepath, String(xmlPrinter.CStr()));
        }

        [[nodiscard]] Status WriteToFile(const StringView& filepath, const toml::table& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::trunc);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const Vector<ubyte>& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::binary | std::ios_base::app);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data.data();
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const String& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::app);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const nlohmann::json& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::app);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const tinyxml2::XMLDocument& data)
        {
            tinyxml2::XMLPrinter xmlPrinter;
            data.Print(&xmlPrinter);

            return AppendToFile(filepath, String(xmlPrinter.CStr()));
        }

        [[nodiscard]] Status AppendToFile(const StringView& filepath, const toml::table& data)
        {
            std::ofstream outputFile(filepath, std::ios_base::out | std::ios_base::app);

            if (!outputFile.is_open())
            {
                return Status::Fail;
            }

            outputFile << data;
            outputFile.close();

            return Status::Success;
        }

        [[nodiscard]] usize GetFileSize(const StringView& filepath)
        {
            std::error_code errorCode;
            const umax fileSize = std::filesystem::file_size(filepath, errorCode);

            return fileSize != std::numeric_limits<umax>::max() ? static_cast<usize>(fileSize) : 0u;
        }

        [[nodiscard]] Status ReadJSON(const StringView& filepath, nlohmann::json& out_data)
        {
            const String fileData = ReadFile(filepath);

            if (fileData.empty())
            {
                return Status::Fail;
            }

            out_data = nlohmann::json::parse(fileData, nullptr, false);

            return out_data.is_discarded() ? Status::Fail : Status::Success;
        }

        [[nodiscard]] Status ReadXML(const StringView& filepath, tinyxml2::XMLDocument& out_document)
        {          
            if (out_document.LoadFile(filepath.data()) != tinyxml2::XML_SUCCESS)
            {
                return Status::Fail;
            }

            return Status::Success;
        }

        [[nodiscard]] Status ReadTOML(const StringView& filepath, toml::table& out_table)
        {
            try
            {
                out_table = toml::parse_file(filepath);
            }
            catch (const toml::parse_error& error)
            {
                return Status::Fail;
            }

            return Status::Success;
        }

        [[nodiscard]] Status SaveToMessagePack(const StringView& filepath, const nlohmann::json& data)
        {
            const Vector<ubyte> messagePackData = nlohmann::json::to_msgpack(data);
            
            return WriteToFile(filepath, messagePackData);
        }

        [[nodiscard]] nlohmann::json ReadMessagePack(const StringView& filepath)
        {
            const Vector<ubyte> messagePackData = ReadFileBytes(filepath);

            if (messagePackData.empty())
            {
                return nlohmann::json{ };
            }

            return nlohmann::json::from_msgpack(messagePackData, false, false);
        }
    }
}