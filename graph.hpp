#pragma once
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <exception>
#include <iostream>

template<class T>
using adjacency_matrix = std::vector<std::vector<T>>;

template<class Label, class Weight>
class Edge {
public:
    using label_type = Label;
    using weight_type = Weight;

    Edge(const label_type& target, const weight_type& weight)
        : m_Target(target), m_Weight(weight)
    {

    }

    Edge(label_type&& target, weight_type&& weight)
        : m_Target(std::move(target)), m_Weight(std::move(weight))
    {

    }

    label_type target() const { return m_Target; }
    weight_type weight() const { return m_Weight; }

private:
    Label m_Target;
    Weight m_Weight;
};

namespace std {
    template<class Label, class Weight>
    struct greater<Edge<Label, Weight>> {
        constexpr bool operator()(const Edge<Label, Weight>& Left, const Edge<Label, Weight>& Right)const
        {
            return Left.weight() > Right.weight();
        }
    };
}

template<
    class Label, class Weight,
    class Hasher = std::hash<Label>,
    class KeyEqual = std::equal_to<Label>
>
class Graph {
    void init(const std::vector<Label>& vertices, adjacency_matrix<Weight>& matrix)
    {

        if (matrix.size() != vertices.size())
            throw std::bad_array_new_length();

        for (size_t vertex = 0; vertex < vertices.size(); ++vertex)
        {
            for (size_t neighbor = 0; neighbor < matrix[vertex].size(); ++neighbor)
            {
                edge_type new_edge(vertices[neighbor], matrix[vertex][neighbor]);
                if (new_edge.weight() != weight_type())
                    m_Graph[vertices[vertex]].push_back(std::move(new_edge));
            }
        }
    }
public:
    using label_type = Label;
    using weight_type = Weight;
    using hasher = Hasher;
    using key_equal = KeyEqual;

    using edge_type = Edge<label_type, weight_type>;
    using map_type = std::list<edge_type>;
    using container_type = std::unordered_map<label_type, map_type, hasher, key_equal>;

    using size_type = typename container_type::size_type;
    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    using map_size_type = typename map_type::size_type;
    using map_iterator = typename map_type::iterator;
    using map_const_iterator = typename map_type::const_iterator;

    Graph() = default;

    Graph(const std::vector<label_type>& vertices, adjacency_matrix<weight_type>& matrix)
    {
        init(vertices, matrix);
    }

    auto add_vertex(const label_type& label)
    {
        return m_Graph.insert(std::make_pair(label, map_type()));
    }
    auto add_vertex(label_type&& label)
    {
        return m_Graph.insert(std::make_pair(std::move(label), map_type()));
    }
    void remove_vertex(const label_type& vertex)
    {
        if (exist(vertex))
        {
            m_Graph.erase(vertex);
            for (auto& _vertex : m_Graph)
                exclude_edges(_vertex.second, vertex);
        }
    }

    void add_edge(const label_type& from, const label_type& to, const weight_type& weight)
    {
        if (exist(from) && exist(to))
            m_Graph[from].push_back(edge_type(to, weight));
    }
    void add_edge(const label_type& from, const label_type& to, const weight_type& weight1, const weight_type& weight2)
    {
        this->add_edge(from, to, weight1);
        this->add_edge(to, from, weight2);
    }
    auto remove_edge(const label_type& from, const label_type& to)
    {
        if (exist(from) && exist(to))
            exclude_edge(m_Graph[from], to);
    }

    bool exist(const label_type& vertex) const
    {
        return m_Graph.find(vertex) != m_Graph.end() ? true : false;
    }

    bool has_edge(const label_type& from, const label_type& to) const
    {
        if (exist(from) && exist(to))
        {
            auto First = m_Graph.at(from).begin();
            const auto Last = m_Graph.at(from).end();
            auto Iter = std::find_if(First, Last, [&to](const auto& edge) { return edge.target() == to; });

            return static_cast<bool>(Iter != Last);
        }
        return false;
    }

    std::list<map_iterator> at_edges(const label_type& from, const label_type& to)
    {
        std::list<map_iterator> result_list;
        if (exist(from) && exist(to))
        {

            auto First = m_Graph[from].begin();
            const auto Last = m_Graph[from].end();

            for (; First != Last; ++First)
            {
                if (First->traget() == to)
                    result_list.push_back(First);
            }
        }
        return result_list;
    }

    size_type size() const { return m_Graph.size(); }
    map_size_type map_size(const label_type& vertex) const { m_Graph.find(vertex)->second.size(); }

    void clear() { m_Graph.clear(); }
    void map_clear(const label_type& vertex) { m_Graph.find(vertex)->second.clear(); }

    iterator begin() { return m_Graph.begin(); }
    iterator end() { return m_Graph.end(); }

    const_iterator cbegin() const { return m_Graph.cbegin(); }
    const_iterator cend() const { return m_Graph.cend(); }

    map_iterator map_begin(const label_type& vertex)
    {
        return  m_Graph.at(vertex).begin();
    }
    map_iterator map_end(const label_type& vertex)
    {
        return m_Graph.at(vertex).end();
    }

    map_const_iterator map_cbegin(const label_type& vertex) const
    {
        return  m_Graph.at(vertex).cbegin();
    }
    map_const_iterator map_cend(const label_type& vertex) const
    {
        return m_Graph.at(vertex).cend();
    }

protected:
    void exclude_edges(map_type& map, const label_type& vertex)
    {
        auto First = map.begin();
        const auto Last = map.end();
        auto Iter = std::remove_if(First, Last, [&vertex](const auto& edge) { return edge.target() == vertex; });
        map.erase(Iter, Last);
    }

private:
    container_type m_Graph;
};

template<class _Label, class _Weight>
std::ostream& operator<<(std::ostream& out, Graph<_Label, _Weight>& graph)
{
    for(auto& vertex: graph)
    {
        out << vertex.first << ": ";
        for(auto& edge: vertex.second)
        {
            out << "(" << edge.target() << ", " << edge.weight() << "), ";
        }
        out << '\n';
    }
    return out;
}
