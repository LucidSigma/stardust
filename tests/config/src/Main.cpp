#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Config can open, read from, and save to a JSON file", "[config]")
{
    sd::Config config;
    REQUIRE(config.Initialise("./", "config_test.json") == sd::Status::Success);

    SECTION("Can read values from a JSON file")
    {
        REQUIRE(config["locale"] == "en_gb");
        REQUIRE(config["graphics"]["multisample"]);
        REQUIRE(config["graphics"]["resolution"]["width"] == 1'920);
    }

    SECTION("Can edit values stored in memory")
    {
        config["locale"] = "en_us";
        config["graphics"]["multisample"] = false;
        config["graphics"]["resolution"]["height"] = 1'440;

        REQUIRE(config["locale"] == "en_us");
        REQUIRE(!config["graphics"]["multisample"]);
        REQUIRE(config["graphics"]["resolution"]["height"] == 1'440);
    }

    SECTION("Can save values in memory to a file")
    {
        config["locale"] = "en_us";
        config["graphics"]["multisample"] = false;
        config["graphics"]["resolution"]["height"] = 1'440;
        REQUIRE(config.Save() == sd::Status::Success);

        sd::Config newConfig;
        REQUIRE(newConfig.Initialise("./", "config_test.json") == sd::Status::Success);

        REQUIRE(newConfig["locale"] == "en_us");
        REQUIRE(!newConfig["graphics"]["multisample"]);
        REQUIRE(newConfig["graphics"]["resolution"]["width"] == 1'920);
        REQUIRE(newConfig["graphics"]["resolution"]["height"] == 1'440);
    
        config["locale"] = "en_gb";
        config["graphics"]["multisample"] = true;
        config["graphics"]["resolution"]["height"] = 1'080;
        REQUIRE(config.Save() == sd::Status::Success);
    }
}