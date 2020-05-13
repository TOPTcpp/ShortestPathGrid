#pragma once
#include "breadth_first_search.hpp"

template<class _Label, class _Weight, class _Visitor = Dijkstra_visitor<_Label, _Weight>>
auto Shortest_path_unchecked(const Graph<_Label, _Weight>& graph, const _Label& source)
{
    _Visitor visitor(graph, source);
    BFS_unchecked(graph, &visitor);
    return visitor.predecessor();
}

template<class _Label, class Container>
auto Construct_shortest_path(const _Label& target, const Predecessor<_Label>& pred, Container& container)
{
    container.clear();
    for (auto vertex = target; vertex != pred.value_default(); vertex = pred.predecessor(vertex))
           container.push_front(vertex);
}

template<class _Label, class _Weight, class _Visitor = Dijkstra_visitor<_Label, _Weight>>
auto Shortest_path(const Graph<_Label, _Weight>& graph, const _Label& source, const _Label& target)
{
    std::list<_Label> path;
    if (graph.exist(source) && graph.exist(target) && source != target)
    {
        Predecessor<_Label> pred(std::move(Shortest_path_unchecked<_Label, _Weight, _Visitor>(graph, source)));
        Construct_shortest_path(target, pred, path);
    }
    return path;
}
