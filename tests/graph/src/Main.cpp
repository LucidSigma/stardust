#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Graphs can be make from nodes and edges, and have information queried about themselves", "[graph]")
{
    sd::ai::Graph<sd::String> graph;

    graph.AddEdge("A", "B", 7.0f, sd::ai::EdgeDirection::Unidirectional);
    graph.AddEdge("B", "D", 1.9f, sd::ai::EdgeDirection::Bidirectional);
    graph.AddEdge("B", "D", 9.0f, sd::ai::EdgeDirection::Bidirectional);
    graph.AddEdge("B", "C", 3.0f, sd::ai::EdgeDirection::Bidirectional);
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
}