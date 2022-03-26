#include <algorithm>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <stardust/Stardust.h>

TEST_CASE("Information about files and directories within a virtual filesystem can be queried", "[vfs]")
{
    REQUIRE(sd::fs::InitialiseApplicationBaseDirectory() == sd::Status::Success);

    #ifdef _WIN32
        REQUIRE(sd::vfs::Initialise((sd::fs::GetApplicationBaseDirectory() + "virtual_filesystem_test.exe").c_str()) == sd::Status::Success);
    #else
        FAIL("Unsupported operating system.");
    #endif

    REQUIRE(sd::vfs::AddToSearchPath(sd::fs::GetApplicationBaseDirectory() + "../test_resources/fs_data.zip") == sd::Status::Success);

    const sd::String dataDirectory = "fs_data/";

    SECTION("Can iterate through all of the files in a virtual directory")
    {
        sd::List<sd::String> files = sd::vfs::GetAllFiles(dataDirectory);
        std::ranges::sort(files);
        REQUIRE(files == sd::List<sd::String>{ "b.txt", "c.txt", "data.json", "data.toml" });

        files.clear();

        for (const auto& file : sd::vfs::IterateFiles(dataDirectory))
        {
            files.push_back(file);
        }

        REQUIRE(files == sd::List<sd::String>{ "b.txt", "c.txt", "data.json", "data.toml" });

        files = sd::vfs::GetAllFilesRecursive(dataDirectory);
        std::ranges::sort(files);
        REQUIRE(files == sd::List<sd::String>{ "a1.txt", "a2.txt", "b.txt", "c.txt", "data.json", "data.toml" });

        files.clear();

        for (const auto& file : sd::vfs::IterateFilesRecursive(dataDirectory))
        {
            files.push_back(file);
        }

        std::ranges::sort(files);
        REQUIRE(files == sd::List<sd::String>{ "a1.txt", "a2.txt", "b.txt", "c.txt", "data.json", "data.toml" });
    }

    SECTION("Can query if files exist and are directories")
    {
        REQUIRE(sd::vfs::DoesPathExist(dataDirectory + "b.txt"));
        REQUIRE(sd::vfs::DoesPathExist(dataDirectory + "c.txt"));
        REQUIRE(sd::vfs::DoesPathExist(dataDirectory + "a/a1.txt"));
        REQUIRE(!sd::vfs::DoesPathExist(dataDirectory + "a.txt"));
    
        REQUIRE(sd::vfs::IsDirectory(dataDirectory + "a"));
        REQUIRE(!sd::vfs::IsDirectory(dataDirectory + "b.txt"));
        REQUIRE(!sd::vfs::IsDirectory(dataDirectory + "c.txt"));
    }

    SECTION("Can read data in files")
    {
        REQUIRE(sd::vfs::ReadFileString(dataDirectory + "b.txt").unwrap() == "This\nis\nsome\ntext.\n");
        REQUIRE(sd::vfs::ReadFileBytes(dataDirectory + "b.txt").unwrap().size() == 19u);

        REQUIRE(sd::vfs::GetFileSize(dataDirectory + "b.txt") == 19u);
    }

    SECTION("Can read special file data formats")
    {
        const sd::JSON jsonData = sd::vfs::ReadJSON(dataDirectory + "data.json").unwrap();
        REQUIRE((!jsonData.is_null() && !jsonData.is_discarded()));
    
        REQUIRE((jsonData["a"].is_boolean() && static_cast<bool>(jsonData["a"])));
        REQUIRE((jsonData["b"].is_number() && static_cast<sd::i32>(jsonData["b"]) == 5));
    
        const sd::TOML tomlData = sd::vfs::ReadTOML(dataDirectory + "data.toml").unwrap();
        REQUIRE(!tomlData.empty());
    
        REQUIRE((tomlData["data"]["a"].is_boolean() && tomlData["data"]["a"].as_boolean()));
        REQUIRE((tomlData["data"]["b"].is_number() && *tomlData["data"]["b"].as_integer() == 5ll));
    }

    sd::vfs::Quit();
}