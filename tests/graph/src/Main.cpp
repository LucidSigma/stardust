#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <stardust/Stardust.h>

TEST_CASE("Graphs can be make from nodes and edges, and have information queried about themselves", "[graph]")
{
    sd::ai::Graph<sd::String> graph;

    SECTION("Can create graphs by adding edges and nodes")
    {
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

        REQUIRE(graph.GetOrder() == 7u);
    }
}