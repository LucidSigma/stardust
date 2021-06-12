#include <benchmark/benchmark.h>
#include <stardust/Stardust.h>

[[nodiscard]] static sd::ai::Graph<char> CreateGraph()
{
    sd::ai::Graph<char> graph;

    graph.AddEdge('A', 'B', 4.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('A', 'C', 2.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('A', 'D', 7.0f, sd::ai::EdgeDirection::Unidirectional);

    graph.AddEdge('B', 'C', 6.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('B', 'E', 6.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('B', 'F', 4.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('B', 'G', 12.0f, sd::ai::EdgeDirection::Unidirectional);

    graph.AddEdge('C', 'G', 7.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('D', 'H', 3.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('F', 'E', 2.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('F', 'I', 13.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('F', 'J', 13.0f, sd::ai::EdgeDirection::Unidirectional);

    graph.AddEdge('G', 'D', 2.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('G', 'F', 1.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('G', 'H', 3.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('G', 'J', 8.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('G', 'K', 4.0f, sd::ai::EdgeDirection::Unidirectional);

    graph.AddEdge('H', 'K', 3.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('H', 'L', 6.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('H', 'M', 18.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('I', 'N', 2.0f, sd::ai::EdgeDirection::Unidirectional);

    graph.AddEdge('J', 'I', 5.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('J', 'N', 3.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('J', 'O', 7.0f, sd::ai::EdgeDirection::Unidirectional);

    graph.AddEdge('K', 'J', 5.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('K', 'P', 13.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge('M', 'P', 3.0f, sd::ai::EdgeDirection::Unidirectional);

    return graph;
}

static void BM_DepthFirstTraversal(benchmark::State& state)
{
    const auto graph = CreateGraph();

    for (auto execution : state)
    {
        const auto result = sd::ai::DepthFirstTraversal(graph, 'A');
    }
}

BENCHMARK(BM_DepthFirstTraversal);

static void BM_BreadthFirstTraversal(benchmark::State& state)
{
    const auto graph = CreateGraph();

    for (auto execution : state)
    {
        const auto result = sd::ai::BreadthFirstTraversal(graph, 'A');
    }
}

BENCHMARK(BM_BreadthFirstTraversal);

static void BM_BestFirstTraversal(benchmark::State& state)
{
    const auto graph = CreateGraph();

    for (auto execution : state)
    {
        const auto result = sd::ai::BestFirstTraversal(graph, 'A');
    }
}

BENCHMARK(BM_BestFirstTraversal);

static void BM_DijkstrasAlgorithm(benchmark::State& state)
{
    const auto graph = CreateGraph();

    for (auto execution : state)
    {
        const auto result = sd::ai::DijkstrasAlgorithm(graph, 'A');
    }
}

BENCHMARK(BM_DijkstrasAlgorithm);

static void BM_DijkstrasAlgorithmSearch(benchmark::State& state)
{
    const auto graph = CreateGraph();

    for (auto execution : state)
    {
        const auto result = sd::ai::DijkstrasAlgorithmSearch(graph, 'A', 'P');
    }
}

BENCHMARK(BM_DijkstrasAlgorithmSearch);

static void BM_AStarSearch(benchmark::State& state)
{
    const auto graph = CreateGraph();

    for (auto execution : state)
    {
        const auto result = sd::ai::AStarSearch(graph, 'A', 'P', sd::ai::HeuristicFunction<char>([](const char&, const char&) { return 0.0f; }));
    }
}

BENCHMARK(BM_AStarSearch);

BENCHMARK_MAIN();