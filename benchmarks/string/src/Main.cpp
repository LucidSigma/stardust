#include <benchmark/benchmark.h>
#include <stardust/Stardust.h>

static void BM_SplitString(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::Split("A B C", ' ');
    }
}

BENCHMARK(BM_SplitString);

static void BM_MakeUpper(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::MakeUpper("hello");
    }
}

BENCHMARK(BM_MakeUpper);

static void BM_MakeLower(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::MakeLower("HELLO");
    }
}

BENCHMARK(BM_MakeLower);

static void BM_RemoveFirstCharacter(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::RemoveFirstCharacter("hello");
    }
}

BENCHMARK(BM_RemoveFirstCharacter);

static void BM_RemoveLastCharacter(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::RemoveLastCharacter("hello");
    }
}

BENCHMARK(BM_RemoveLastCharacter);

static void BM_RemoveFirstAndLastCharacters(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::RemoveFirstAndLastCharacters("hello");
    }
}

BENCHMARK(BM_RemoveFirstAndLastCharacters);

static void BM_LeftTrim(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::LeftTrim("    hello");
    }
}

BENCHMARK(BM_LeftTrim);

static void BM_RightTrim(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::RightTrim("hello    ");
    }
}

BENCHMARK(BM_RightTrim);

static void BM_Trim(benchmark::State& state)
{
    for (auto execution : state)
    {
        const auto result = sd::string::Trim("    hello    ");
    }
}

BENCHMARK(BM_Trim);

BENCHMARK_MAIN();