#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <ranges>
#include <string>

#include <stardust/Stardust.h>

TEST_CASE("Global resources can be managed via keys", "[global_resources]")
{
    sd::GlobalResources globalResources{ };

    for (const auto number : std::ranges::iota_view(1, 6))
    {
        globalResources.Set(std::to_string(number), number);
    }

    for (const auto& letter : { "a", "b", "c" })
    {
        globalResources.Set(letter, sd::String(letter));
    }

    globalResources.Set("true", true);
    globalResources.Set("false", false);

    SECTION("Can determine if a key exists")
    {
        REQUIRE(globalResources.HasKey("1"));
        REQUIRE(globalResources.HasKey("3"));
        REQUIRE(globalResources.HasKey("5"));
        REQUIRE(globalResources.HasKey("a"));
        REQUIRE(globalResources.HasKey("c"));
        REQUIRE(globalResources.HasKey("true"));

        REQUIRE(!globalResources.HasKey("6"));
        REQUIRE(!globalResources.HasKey("d"));
        REQUIRE(!globalResources.HasKey("maybe"));
    }

    SECTION("Can retrieve values with a key")
    {
        REQUIRE(globalResources.Get<sd::i32>("1") == 1);
        REQUIRE(globalResources.Get<sd::i32>("2") == 2);
        REQUIRE(globalResources.Get<sd::i32>("3") == 3);
        REQUIRE(globalResources.Get<sd::i32>("4") == 4);
        REQUIRE(globalResources.Get<sd::i32>("5") == 5);

        REQUIRE(globalResources.Get<sd::String>("a") == "a");
        REQUIRE(globalResources.Get<sd::String>("b") == "b");
        REQUIRE(globalResources.Get<sd::String>("c") == "c");

        REQUIRE(globalResources.Get<bool>("true"));
        REQUIRE(!globalResources.Get<bool>("false"));
    }

    SECTION("Can remove keys from itself")
    {
        globalResources.Remove("1");
        REQUIRE(!globalResources.HasKey("1"));

        globalResources.Remove("b");
        REQUIRE(!globalResources.HasKey("b"));

        globalResources.Remove("false");
        REQUIRE(!globalResources.HasKey("false"));

        globalResources.Clear();
        REQUIRE(!globalResources.HasKey("2"));
        REQUIRE(!globalResources.HasKey("3"));
        REQUIRE(!globalResources.HasKey("4"));
        REQUIRE(!globalResources.HasKey("5"));
        REQUIRE(!globalResources.HasKey("a"));
        REQUIRE(!globalResources.HasKey("c"));
        REQUIRE(!globalResources.HasKey("true"));
    }
}
