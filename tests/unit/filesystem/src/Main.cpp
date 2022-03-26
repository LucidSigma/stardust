#include <algorithm>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <stardust/Stardust.h>

TEST_CASE("Information about files and directories can be queried", "[filesystem]")
{
    REQUIRE(sd::fs::InitialiseApplicationBaseDirectory() == sd::Status::Success);
    const sd::String dataDirectory = sd::fs::GetApplicationBaseDirectory() + "../test_resources/fs_data/";

    SECTION("Can iterate through all of the files in a directory")
    {
        sd::List<sd::String> files = sd::fs::GetAllFiles(dataDirectory);
        std::ranges::sort(files);
        REQUIRE(files == sd::List<sd::String>{ "b.txt", "c.txt", "data.json", "data.toml" });

        files.clear();

        for (const auto& file : sd::fs::IterateFiles(dataDirectory))
        {
            files.push_back(file);
        }

        REQUIRE(files == sd::List<sd::String>{ "b.txt", "c.txt", "data.json", "data.toml" });

        files = sd::fs::GetAllFilesRecursive(dataDirectory);
        std::ranges::sort(files);
        REQUIRE(files == sd::List<sd::String>{ "a1.txt", "a2.txt", "b.txt", "c.txt", "data.json", "data.toml" });

        files.clear();

        for (const auto& file : sd::fs::IterateFilesRecursive(dataDirectory))
        {
            files.push_back(file);
        }

        REQUIRE(files == sd::List<sd::String>{ "a1.txt", "a2.txt", "b.txt", "c.txt", "data.json", "data.toml" });
    }

    SECTION("Can query if files exist and are directories")
    {
        REQUIRE(sd::fs::DoesPathExist(dataDirectory + "b.txt"));
        REQUIRE(sd::fs::DoesPathExist(dataDirectory + "c.txt"));
        REQUIRE(sd::fs::DoesPathExist(dataDirectory + "a/a1.txt"));
        REQUIRE(!sd::fs::DoesPathExist(dataDirectory + "a.txt"));

        REQUIRE(sd::fs::IsDirectory(dataDirectory + "a"));
        REQUIRE(!sd::fs::IsDirectory(dataDirectory + "b.txt"));
        REQUIRE(!sd::fs::IsDirectory(dataDirectory + "c.txt"));
    }

    SECTION("Can query for the stem and extension of a file")
    {
        REQUIRE(sd::fs::HasStem("a.txt"));
        REQUIRE(sd::fs::GetStem("a.txt") == "a");

        REQUIRE(sd::fs::HasExtension("a.txt"));
        REQUIRE(!sd::fs::HasExtension("a"));
        REQUIRE(!sd::fs::HasExtension(".txt"));
        REQUIRE(sd::fs::GetExtension("a.txt") == ".txt");

    #ifdef _WIN32
        REQUIRE(sd::fs::GetDirectorySeparator() == "\\");
    #else
        REQUIRE(sd::fs::GetDirectorySeparator() == "/");
    #endif
    }

    SECTION("Can read data in files")
    {
        REQUIRE(sd::fs::ReadFile(dataDirectory + "b.txt").unwrap() == "This\nis\nsome\ntext.\n");
        REQUIRE(sd::fs::ReadFileLines(dataDirectory + "b.txt").unwrap() == sd::List<sd::String>{ "This", "is", "some", "text." });
        REQUIRE(sd::fs::ReadFileBytes(dataDirectory + "b.txt").unwrap().size() == 19u);

        sd::List<sd::String> fileLines{ };

        for (const auto& line : sd::fs::IterateFileLines(dataDirectory + "b.txt"))
        {
            fileLines.push_back(line);
        }

        REQUIRE(fileLines == sd::List<sd::String>{ "This", "is", "some", "text." });

        REQUIRE(sd::fs::GetFileSize(dataDirectory + "b.txt") == 19u);
    }

    SECTION("Can read special file data formats")
    {
        const sd::JSON jsonData = sd::fs::ReadJSON(dataDirectory + "data.json").unwrap();
        REQUIRE((!jsonData.is_null() && !jsonData.is_discarded()));

        REQUIRE((jsonData["a"].is_boolean() && static_cast<bool>(jsonData["a"])));
        REQUIRE((jsonData["b"].is_number() && static_cast<sd::i32>(jsonData["b"]) == 5));

        const sd::TOML tomlData = sd::fs::ReadTOML(dataDirectory + "data.toml").unwrap();
        REQUIRE(!tomlData.empty());

        REQUIRE((tomlData["data"]["a"].is_boolean() && tomlData["data"]["a"].as_boolean()));
        REQUIRE((tomlData["data"]["b"].is_number() && *tomlData["data"]["b"].as_integer() == 5ll));
    }
}