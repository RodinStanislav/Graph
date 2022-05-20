#pragma once

#include <set>
#include <vector>

class AbstractGraph {
    friend void PrintGraph(const AbstractGraph&);
public:
    virtual ~AbstractGraph() = default;

    void addNode(std::size_t id);
    virtual void addLink(std::size_t from, std::size_t to) = 0;

    void removeNode(std::size_t id);
    virtual void removeLink(std::size_t from, std::size_t to) = 0;

    void clear();

    std::size_t getCountOfNodes() const;
    bool hasPath(std::size_t from, std::size_t to) const;
    std::vector<std::size_t> getNeighbours(std::size_t id) const;
protected:
    AbstractGraph() = default;

    void removeLinkByIndex(std::size_t index);
    void addNewLinkByIndex(std::size_t index);
    bool hasPathByIndex(std::size_t indexFrom, std::size_t indexTo) const;
    bool hasPathByIndex(std::size_t indexFrom, std::size_t indexTo, std::vector<bool>& visited) const;
    std::size_t getNodeIndex(std::size_t id) const;
    std::vector<std::size_t> getNeighboursByIndex(std::size_t index) const;
    void setWeightByIndices(std::size_t indexFrom, std::size_t indexTo, int value);

    std::set<std::size_t> m_nodes;
    std::vector<std::vector<int>> m_links; //list? deque?
};

class Graph : public AbstractGraph {
public:
    Graph() = default;
    virtual ~Graph() = default;

    virtual void addLink(std::size_t from, std::size_t to) override;
    virtual void removeLink(std::size_t from, std::size_t to) override;
};

class OrientedGraph : public Graph {
public:
    OrientedGraph() = default;
    virtual ~OrientedGraph() = default;

    void addLink(std::size_t from, std::size_t to) override;
    void removeLink(std::size_t from, std::size_t to) override;
};

class WeightedGraph : public Graph {
public:
    WeightedGraph() = default;
    virtual ~WeightedGraph() = default;

    void addLink(std::size_t from, std::size_t to, int value);
    void setWeight(std::size_t from, std::size_t to, int value);
};

class WeightedOrientedGraph final : 
    virtual public OrientedGraph, virtual public WeightedGraph {
public:
    WeightedOrientedGraph() = default;
    virtual ~WeightedOrientedGraph() = default;
};

void PrintGraph(const AbstractGraph&);