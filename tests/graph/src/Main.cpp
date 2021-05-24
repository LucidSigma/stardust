#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Graphs can be make from nodes and edges, and have information queried about themselves", "[graph]")
{
    sd::ai::Graph<sd::String> graph;

    graph.AddEdge("A", "B", 7.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge("B", "C", 3.0f, sd::ai::EdgeDirection::Bidirectional);
    graph.AddEdge("B", "D", 1.9f, sd::ai::EdgeDirection::Bidirectional);
    graph.AddEdge("B", "D", 9.0f, sd::ai::EdgeDirection::Bidirectional);
    graph.AddEdge("C", "A", 2.5f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge("C", "D", 0.4f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge("D", "F", 11.1f, sd::ai::EdgeDirection::Bidirectional);
    graph.AddEdge("E", "C", 6.3f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge("E", "D", 1.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdgelessNode("G");

    SECTION("Can query information about a graph's edges and nodes")
    {
        REQUIRE(graph.GetNodes().size() == 7u);
        REQUIRE(graph.GetOrder() == 7u);
        REQUIRE(graph.GetSize() == 9u);

        REQUIRE(graph.GetDegree("A") == 1u);
        REQUIRE(graph.GetDegree("B") == 3u);
        REQUIRE(graph.GetDegree("C") == 3u);
        REQUIRE(graph.GetDegree("D") == 3u);
        REQUIRE(graph.GetDegree("E") == 2u);
        REQUIRE(graph.GetDegree("F") == 1u);
        REQUIRE(graph.GetDegree("G") == 0u);
    }

    SECTION("Can query if nodes exist and are connected")
    {
        REQUIRE(graph.HasNode("A"));
        REQUIRE(graph.HasNode("D"));
        REQUIRE(!graph.HasNode("H"));
        REQUIRE(!graph.HasNode("K"));

        REQUIRE(graph.DoNodesConnect("A", "B"));
        REQUIRE(graph.DoNodesConnect("C", "D"));
        REQUIRE(!graph.DoNodesConnect("D", "C"));
        REQUIRE(!graph.DoNodesConnect("G", "F"));
    }

    SECTION("Can get all adjacent nodes/weights from a source node")
    {
        auto adjacentEdges = graph.GetAdjacentNodes("B");
        REQUIRE(adjacentEdges.size() == 3u);

        const auto efficientAdjacentEdges = graph.GetEfficientAdjacentNodes("B");
        REQUIRE(efficientAdjacentEdges.size() == 2u);

        adjacentEdges = graph.GetAdjacentNodes("D");
        sd::f32 edgeWeightSum = 0.0f;
        
        for (const auto& [node, weight] : adjacentEdges)
        {
            edgeWeightSum += weight;
        }

        REQUIRE(adjacentEdges.size() == 3u);
        REQUIRE(edgeWeightSum == 22.0f);
    }

    SECTION("Can remove nodes from a graph")
    {
        graph.RemoveNode("D");

        REQUIRE(!graph.HasNode("D"));

        REQUIRE(graph.GetOrder() == 6u);
        REQUIRE(graph.GetSize() == 4u);

        REQUIRE(graph.GetDegree("B") == 1u);
        REQUIRE(graph.GetDegree("C") == 2u);
        REQUIRE(graph.GetDegree("E") == 1u);
        REQUIRE(graph.GetDegree("F") == 0u);
    }

    SECTION("Can remove all edges between nodes")
    {
        graph.RemoveEdges("A", "B");
        graph.RemoveEdges("B", "C");

        REQUIRE(graph.GetSize() == 7u);

        REQUIRE(graph.GetDegree("A") == 0u);
        REQUIRE(graph.GetDegree("B") == 2u);
        REQUIRE(graph.GetDegree("C") == 2u);
    }

    SECTION("Can traverse a graph")
    {
        auto depthFirstTraversal = sd::ai::DepthFirstTraversal<sd::String>(graph, "A");
        REQUIRE(depthFirstTraversal == sd::Vector<sd::String>{ "A", "B", "D", "F", "C" });

        depthFirstTraversal = sd::ai::DepthFirstTraversal<sd::String>(graph, "E");
        REQUIRE(depthFirstTraversal == sd::Vector<sd::String>{ "E", "D", "F", "B", "C", "A" });

        depthFirstTraversal = sd::ai::DepthFirstTraversal<sd::String>(graph, "E", "B");
        REQUIRE(depthFirstTraversal == sd::Vector<sd::String>{ "E", "D", "F", "B" });

        auto breadthFirstTraversal = sd::ai::BreadthFirstTraversal<sd::String>(graph, "A");
        REQUIRE(breadthFirstTraversal == sd::Vector<sd::String>{ "A", "B", "C", "D", "F" });

        breadthFirstTraversal = sd::ai::BreadthFirstTraversal<sd::String>(graph, "E");
        REQUIRE(breadthFirstTraversal == sd::Vector<sd::String>{ "E", "C", "D", "A", "B", "F" });

        breadthFirstTraversal = sd::ai::BreadthFirstTraversal<sd::String>(graph, "E", "B");
        REQUIRE(breadthFirstTraversal == sd::Vector<sd::String>{ "E", "C", "D", "A", "B" });

        auto bestFirstTraversal = sd::ai::BestFirstTraversal<sd::String>(graph, "A");
        REQUIRE(bestFirstTraversal == sd::Vector<sd::String>{ "A", "B", "D", "C", "F" });

        bestFirstTraversal = sd::ai::BestFirstTraversal<sd::String>(graph, "E");
        REQUIRE(bestFirstTraversal == sd::Vector<sd::String>{ "E", "D", "B", "C", "A", "F" });

        bestFirstTraversal = sd::ai::BestFirstTraversal<sd::String>(graph, "E", "B");
        REQUIRE(bestFirstTraversal == sd::Vector<sd::String>{ "E", "D", "B" });
    }

    SECTION("Can pathfind in a graph")
    {
        auto dijkstrasAlgorithm = sd::ai::DijkstrasAlgorithm<sd::String>(graph, "E", "A");
        REQUIRE(dijkstrasAlgorithm == sd::Vector<sd::String>{ "E", "D", "B", "C", "A" });

        dijkstrasAlgorithm = sd::ai::DijkstrasAlgorithm<sd::String>(graph, "A", "D");
        REQUIRE(dijkstrasAlgorithm == sd::Vector<sd::String>{ "A", "B", "D" });

        dijkstrasAlgorithm = sd::ai::DijkstrasAlgorithm<sd::String>(graph, "B", "E");
        REQUIRE(dijkstrasAlgorithm.empty());

        std::function<sd::f32(const sd::String&, const sd::String&)> heuristicFunction = [](const sd::String& currentNode, const sd::String& goalNode) -> sd::f32
        {
            return sd::HashMap<sd::String, sd::f32>{
                { "A", 0.0f },
                { "B", 5.5f },
                { "C", 2.5f },
                { "D", 7.4f },
                { "E", 8.4f },
                { "F", 18.4f },
                { "G", std::numeric_limits<sd::f32>::max() },
            }.at(currentNode);
        };

        auto aStarSearch = sd::ai::AStarSearch<sd::String>(graph, "E", "A", heuristicFunction);
        REQUIRE(aStarSearch == sd::Vector<sd::String>{ "E", "D", "B", "C", "A" });

        heuristicFunction = [](const sd::String& currentNode, const sd::String& goalNode) -> sd::f32
        {
            return sd::HashMap<sd::String, sd::f32>{
                { "A", 8.9f },
                { "B", 1.9f },
                { "C", 0.4f },
                { "D", 0.0f },
                { "E", 1.0f },
                { "F", 11.1f },
                { "G", std::numeric_limits<sd::f32>::max() },
            }.at(currentNode);
        };

        aStarSearch = sd::ai::AStarSearch<sd::String>(graph, "A", "D", heuristicFunction);
        REQUIRE(aStarSearch == sd::Vector<sd::String>{ "A", "B", "D" });

        aStarSearch = sd::ai::AStarSearch<sd::String>(graph, "B", "E", heuristicFunction);
        REQUIRE(aStarSearch.empty());
    }
}