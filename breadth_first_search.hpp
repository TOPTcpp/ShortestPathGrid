#pragma once
#include "visitor.hpp"

template<class _Label, class _Weight, class _Queue>
void BFS_unchecked(const Graph<_Label, _Weight>& graph, Visitor_with_queue<_Label, _Weight, _Queue>* visitor)
{
    while (!visitor->empty())
    {
        auto processed_vertex = visitor->extract_vertex();

        auto edges_list_begin = graph.map_cbegin(processed_vertex.target());
        auto edges_list_end = graph.map_cend(processed_vertex.target());

        visitor->handle(edges_list_begin, edges_list_end, processed_vertex);
    }
}

template<class _Label, class _Weight>
auto Breadth_first_search(const Graph<_Label, _Weight>& graph, const _Label& source)
{
    BFS_visitor<_Label, _Weight> visitor(graph, source);
    if (graph.exist(source))
        BFS_unchecked(graph, &visitor);
    return visitor;
}
