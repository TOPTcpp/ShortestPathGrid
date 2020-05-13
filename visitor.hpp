#pragma once
#include "graph.hpp"
#include <type_traits>
#include <queue>
#include <limits>
#include <map>
#include <type_traits>
#include <cassert>

enum class color_type { white, gray, black };

template<class Label, class Container = std::map <Label, Label>>
class Predecessor
{
    template<class GraphIterator>
    void assign_container(GraphIterator First, GraphIterator Last)
    {
        std::for_each(First, Last,
            [&](const auto& vertex) {
                const auto label = vertex.first;
                m_Predecessor.insert(std::make_pair(label, m_Default));
            }
        );
    }

public:
    static_assert(std::is_default_constructible<Label>::value, "Type Label does not have a default constructor");

    Predecessor(){};

    template<class Weight>
    Predecessor(const Graph<Label, Weight>& graph)
    {
        assign_container(graph.cbegin(), graph.cend());
    }

    explicit Predecessor(const Container& container)
        : m_Predecessor(container)
    {

    }

    explicit Predecessor(Container&& container)
        : m_Predecessor(std::move(container))
    {

    }

    virtual ~Predecessor() {}

    template<class Weight>
    Predecessor<Label, Container>& operator=(const Graph<Label, Weight>& graph)
    {
        m_Predecessor.clear();
        assign_container(graph.cbegin(), graph.cend());
        return *this;
    }

    Predecessor<Label, Container>& operator=(const Container& container)
    {
        m_Predecessor.clear();
        m_Predecessor = container;
        return *this;
    }

    Predecessor<Label, Container>& operator=(Container&& container)
    {
        m_Predecessor.clear();
        m_Predecessor = std::move(container);
        return *this;
    }

    void update(const Label& processed_vertex, const Label& neighbor_vertex)
    {
        m_Predecessor[neighbor_vertex] = processed_vertex;
    }

    auto value_default() const { return m_Default; }

    auto predecessor(const Label& vertex) const { return m_Predecessor.at(vertex); }
    auto predecessor() const  { return m_Predecessor; }

private:
    Container m_Predecessor;
    Label m_Default = std::is_arithmetic<Label>::value ? std::numeric_limits<Label>::max() : Label();
};

template<class Label, class Container = std::map<Label, color_type>>
class Color
{
    template<class GraphIterator>
    void assign_container(GraphIterator First, GraphIterator Last)
    {
        std::for_each(First, Last,
            [&](const auto& vertex) {
                const auto label = vertex.first;
                m_Color.insert(std::make_pair(label, color_type::white));
            }
        );
    }

public:
    Color() {};

    template<class Weight>
    Color(const Graph<Label, Weight>& graph, const Label& source)
    {
        assign_container(graph.cbegin(), graph.cedn());
        discovered(source);
    }

    explicit Color(const Container& other_container)
        : m_Color(other_container)
    {

    }

    explicit Color(Container&& other_container)
        : m_Color(std::move(other_container))
    {

    }

    virtual ~Color() {}

    template<class Weight>
    Color<Label, Container>& operator=(const Graph<Label, Weight>& graph)
    {
        m_Color.clear();
        assign_container(graph.cbegin(), graph.cend());
        return *this;
    }

    Color<Label, Container>& operator=(const Container& other_container)
    {
        m_Color.clear();
        m_Color = other_container;
        return *this;
    }

    Color<Label, Container>& operator=(Container&& other_container)
    {
        m_Color.clear();
        m_Color = std::move(other_container);
        return *this;
    }

    void discovered(const Label& vertex) { m_Color[vertex] = color_type::gray; }
    void visited(const Label& vertex) { m_Color[vertex] = color_type::black; }

    auto color(const Label& vertex) const { return m_Color.at(vertex); }
    auto color() const { return m_Color; }
private:
    Container m_Color;
};

template<class Label, class Weight, class Container = std::map<Label, Weight>>
class Distance
{
    template<class GraphIterator>
    void assign_container(GraphIterator First, GraphIterator Last)
    {
        std::for_each(First, Last,
            [&](const auto& vertex) {
                const auto label = vertex.first;
                m_Distance.insert(std::make_pair(label, std::numeric_limits<Weight>::max()));
            }
        );

    }
public:
    static_assert(std::is_arithmetic<Weight>::value, "Type of Weight is not arithmetic.");
    using edge_type = Edge<Label, Weight>;

    Distance() = delete;

    Distance(const Graph<Label, Weight>& graph, const Label& source)
    {
        assign_container(graph.cbegin(), graph.cend());
        m_Distance[source] = static_cast<Weight>(0);
    }

    explicit Distance(const Container& other_container)
        : m_Distance(other_container)
    {

    }

    explicit Distance(Container&& other_container)
        : m_Distance(std::move(other_container))
    {

    }

    virtual ~Distance() {}

    Distance<Label, Weight, Container>& operator=(const Graph<Label, Weight>& graph)
    {
        m_Distance.clear();
        assign_container(graph.cbegin(), graph.cend());
        return *this;
    }

    Distance<Label, Weight, Container>& operator=(const Container& other_container)
    {
        m_Distance.clear();
        m_Distance = other_container;
        return *this;
    }

    Distance<Label, Weight, Container>& operator=(Container&& other_container)
    {
        m_Distance.clear();
        m_Distance = std::move(other_container);
        return *this;
    }

    void update(const edge_type& processed_vertex, const edge_type& neighbor_vertex)
    {
        m_Distance[neighbor_vertex.target()] = processed_vertex.weight() + neighbor_vertex.weight();
    }

    void update(const Label& vertex, const Weight& weight)
    {
        m_Distance[vertex] = weight;
    }

    auto distance(const Label& vertex) const { return m_Distance.at(vertex); }
    auto distance() const { return m_Distance; }
private:
    Container m_Distance;
};

template<class Label, class Weight>
class Visitor : public Predecessor<Label>, public Distance<Label, Weight>
{
public:
    using label_type = Label;
    using weight_type = Weight;

    using edge_type = Edge<label_type, weight_type>;
    using base_predecessor = Predecessor<Label>;
    using base_distance = Distance<Label, Weight>;

    using graph_type = Graph<label_type, weight_type>;

    Visitor() = delete;

    Visitor(const graph_type& graph, const label_type& source)
        : base_predecessor(graph), base_distance(graph, source)
    {

    }
    virtual ~Visitor() {}

    void update(const edge_type& processed_vertex, const edge_type& neighbor_vertex)
    {
        base_distance::update(processed_vertex, neighbor_vertex);
        base_predecessor::update(processed_vertex.target(), neighbor_vertex.target());
    }
};

template<class Label, class Weight, class Queue>
class Visitor_with_queue : public Visitor<Label, Weight>
{
public:
    using edge_type = Edge<Label, Weight>;
    using base_visitor = Visitor<Label, Weight>;
    using queue_type = Queue;

    using edges_const_iterator = typename Graph<Label, Weight>::map_const_iterator;

    Visitor_with_queue() = delete;

    Visitor_with_queue(const Graph<Label, Weight>& graph, const Label& source)
        : base_visitor(graph,source)
    {
        push(source);
    }
    virtual ~Visitor_with_queue() {}

    virtual void handle(edges_const_iterator& First, edges_const_iterator& Last, const edge_type& processed_vertex) = 0;

    virtual edge_type extract_vertex() = 0;

    void push(const Label& vertex)
    {
        m_Queue.emplace(edge_type(vertex, this->distance(vertex)));
    }
    auto empty() const { return m_Queue.empty(); }

protected:
    queue_type m_Queue;
};

template<class Label, class Weight>
class Dijkstra_visitor
    : public Visitor_with_queue<Label, Weight, std::priority_queue<Edge<Label, Weight>, std::vector<Edge<Label, Weight>>, std::greater<Edge<Label, Weight>>>>
{
public:
    using edge_type = Edge<Label, Weight>;
    using queue_type = std::priority_queue<edge_type, std::vector<edge_type>, std::greater<edge_type>>;
    using base_visitor = Visitor_with_queue<Label, Weight, queue_type>;

    using edges_const_iterator = typename base_visitor::edges_const_iterator;

    Dijkstra_visitor() = delete;

    Dijkstra_visitor(const Graph<Label, Weight>& graph, const Label& source)
        : base_visitor(graph, source)
    {

    }

    virtual ~Dijkstra_visitor() {}

    virtual void handle(edges_const_iterator& First, edges_const_iterator& Last, const edge_type& processed_vertex) override
    {
        std::for_each(First, Last,
            [&](const auto& neighbor) {
                auto total_distance = processed_vertex.weight() + neighbor.weight();

                if (total_distance < this->distance(neighbor.target()))
                {
                    base_visitor::update(processed_vertex, neighbor);
                    base_visitor::push(neighbor.target());
                }
            });
    }

    virtual edge_type extract_vertex() override
    {
        edge_type vertex = base_visitor::m_Queue.top();
        base_visitor::m_Queue.pop();
        return vertex;
    }

};

template<class Label, class Weight>
class BFS_visitor final
    : public Visitor_with_queue<Label, Weight, std::queue<Edge<Label, Weight>>>, public Color<Label>
{
public:
    using edge_type = Edge<Label, Weight>;
    using queue_type = std::queue<edge_type>;
    using base_visitor = Visitor_with_queue<Label, Weight, queue_type>;
    using base_color = Color<Label>;

    using edges_const_iterator = typename base_visitor::edges_const_iterator;

    BFS_visitor() = delete;

    BFS_visitor(const Graph<Label, Weight>& graph, const Label& source)
        : base_visitor(graph, source), base_color(graph, source)
    {

    }
    ~BFS_visitor() {}

    void handle(edges_const_iterator& First, edges_const_iterator& Last, const edge_type& processed_vertex) override
    {
        std::for_each(First, Last,
            [&](const auto& neighbor) {
                if (this->color(neighbor.target()) == color_type::white)
                {
                    this->discovered(neighbor.target());
                    base_visitor::update(processed_vertex, neighbor);
                    base_visitor::push(neighbor.target());
                }
            });
        this->visited(processed_vertex.target());
    }

    edge_type extract_vertex() override
    {
        edge_type vertex = base_visitor::m_Queue.front();
        base_visitor::m_Queue.pop();
        return vertex;
    }
};

// TODO
template<class Label, class Weight>
class Prim_visitor : public Dijkstra_visitor<Label, Weight>
{

};
