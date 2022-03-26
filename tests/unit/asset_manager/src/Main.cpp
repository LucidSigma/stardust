#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ranges>
#include <string>

#include <stardust/Stardust.h>

TEST_CASE("Assets can be managed and queried", "[asset_manager]")
{
    sd::AssetManager<sd::i32> assets{ };
    
    for (const auto number : std::ranges::iota_view(1, 6))
    {
        assets.Add(std::to_string(number), number);
    }

    SECTION("Can determine how many assets exist")
    {
        REQUIRE(assets.GetAssetCount() == 5u);
    }

    SECTION("Can query if assets exist via a key")
    {
        REQUIRE(assets.Has("1"));
        REQUIRE(assets.Has("3"));
        REQUIRE(assets.Has("5"));

        REQUIRE(!assets.Has("6"));
        REQUIRE(!assets.Has("8"));
        REQUIRE(!assets.Has("10"));
    }

    SECTION("Can remove assets from itself")
    {
        assets.Remove("3");
        REQUIRE(assets.GetAssetCount() == 4u);

        assets.Clear();
        REQUIRE(assets.GetAssetCount() == 0u);
    }

    SECTION("Can retrieve assets with a key")
    {
        REQUIRE(assets.Get("1") == 1);
        REQUIRE(assets.Get("2") == 2);
        REQUIRE(assets["3"] == 3);
        REQUIRE(assets["4"] == 4);

        REQUIRE(*assets.GetAssets().at("5") == 5);
    }

    SECTION("Can iterate over the contained assets")
    {
        sd::i32 assetCounter = 1;

        for (const auto& asset : assets.Iterate())
        {
            REQUIRE(asset == assetCounter);
            ++assetCounter;
        }

        for (assetCounter = 1; const auto & [key, asset] : assets.GetAssets())
        {
            REQUIRE(key == std::to_string(assetCounter));
            REQUIRE(*asset == assetCounter);
            ++assetCounter;
        }

        for (assetCounter = 1; const auto& [key, asset] : assets)
        {
            REQUIRE(key == std::to_string(assetCounter));
            REQUIRE(*asset == assetCounter);
            ++assetCounter;
        }
    }
}
