#include "Graph.h"

#include <iostream>
#include <string>

void AbstractGraph::addNode(std::size_t id) {
    if (id == std::numeric_limits<std::size_t>::max()) {
        return;
    }

    m_nodes.insert(id);
    addNewLinkByIndex(getNodeIndex(id));
}

void AbstractGraph::removeNode(std::size_t id) {
    if (id == std::numeric_limits<std::size_t>::max()) {
        return;
    }

    removeLinkByIndex(getNodeIndex(id));
    m_nodes.erase(id);
}

void AbstractGraph::clear() {
    m_nodes.clear();
    m_links.clear();
}

std::size_t AbstractGraph::getCountOfNodes() const {
    return m_nodes.size();
}

bool AbstractGraph::hasPath(std::size_t from, std::size_t to) const {
    return hasPathByIndex(getNodeIndex(from), getNodeIndex(to));
}

std::vector<std::size_t> AbstractGraph::getNeighbours(std::size_t id) const {
    std::vector<std::size_t> result;

    for (std::size_t nodeId = 0; auto & node : m_nodes) {
        if (node == id) {
            auto paths = getNeighboursByIndex(nodeId);

            for (auto path : paths) {
                auto currentIt = m_nodes.begin();
                for (std::size_t i = 0; i < path; i++) {
                    currentIt++;
                }

                result.emplace_back(*currentIt);
            }

            break;
        }

        nodeId++;
    }

    return result;
}

void AbstractGraph::removeLinkByIndex(std::size_t index) {
    if (index == std::numeric_limits<std::size_t>::max()) {
        return;
    }

    std::vector<std::vector<int>> newPaths(m_nodes.size() - 1);

    for (auto& row : newPaths) {
        row.resize(m_nodes.size() - 1, 0);
    }

    if (index == 0) {
        for (std::size_t rowIndexI = 1; rowIndexI < m_links.size(); rowIndexI++) {
            for (std::size_t rowIndexJ = 1; rowIndexJ < m_links.size(); rowIndexJ++) {
                newPaths[rowIndexI - 1][rowIndexJ - 1] = m_links[rowIndexI][rowIndexJ];
            }
        }
    }
    else if (index == m_links.size() - 1) {
        for (std::size_t rowIndexI = 0; rowIndexI < m_links.size(); rowIndexI++) {
            for (std::size_t rowIndexJ = 0; rowIndexJ < m_links.size(); rowIndexJ++) {
                newPaths[rowIndexI][rowIndexJ] = m_links[rowIndexI][rowIndexJ];
            }
        }
    }
    else {
        std::size_t newIndexI = 0;
        std::size_t newIndexJ = 0;

        for (std::size_t i = 0; i < m_links.size(); i++) {
            if (i == index) {
                continue;
            }

            for (std::size_t j = 0; j < m_links.size(); j++) {
                if (j == index) {
                    continue;
                }

                newPaths[newIndexI][newIndexJ] = m_links[i][j];

                newIndexJ++;
            }

            newIndexI++;
            newIndexJ = 0;
        }
    }

    m_links = std::move(newPaths);
}

void AbstractGraph::addNewLinkByIndex(std::size_t index) {
    if (index == std::numeric_limits<std::size_t>::max()) {
        return;
    }

    std::vector<std::vector<int>> newPaths(m_nodes.size());

    for (auto& row : newPaths) {
        row.resize(m_nodes.size(), 0);
    }

    if (index == 0) {
        for (std::size_t rowIndexI = 1; rowIndexI < m_links.size() + 1; rowIndexI++) {
            for (std::size_t rowIndexJ = 1; rowIndexJ < m_links.size() + 1; rowIndexJ++) {
                newPaths[rowIndexI][rowIndexJ] = m_links[rowIndexI - 1][rowIndexJ - 1];
            }
        }
    }
    else if (index == m_links.size() - 1) {
        for (std::size_t rowIndexI = 0; rowIndexI < m_links.size(); rowIndexI++) {
            for (std::size_t rowIndexJ = 0; rowIndexJ < m_links.size(); rowIndexJ++) {
                newPaths[rowIndexI][rowIndexJ] = m_links[rowIndexI][rowIndexJ];
            }
        }
    }
    else {
        std::size_t oldIndexI = 0;
        std::size_t oldIndexJ = 0;

        for (std::size_t i = 0; i < newPaths.size(); i++) {
            if (i == index) {
                continue;
            }

            for (std::size_t j = 0; j < newPaths.size(); j++) {
                if (j == index) {
                    continue;
                }

                newPaths[i][j] = m_links[oldIndexI][oldIndexJ];

                oldIndexJ++;
            }

            oldIndexI++;
            oldIndexJ = 0;
        }
    }

    m_links = std::move(newPaths);
}

bool AbstractGraph::hasPathByIndex(std::size_t indexFrom, std::size_t indexTo) const {
    if (indexFrom == std::numeric_limits<std::size_t>::max() ||
        indexTo == std::numeric_limits<std::size_t>::max()) {
        return false;
    }

    std::vector<bool> visited(m_nodes.size());
    return hasPathByIndex(indexFrom, indexTo, visited);
}

bool AbstractGraph::hasPathByIndex(std::size_t indexFrom, std::size_t indexTo, std::vector<bool>& visited) const {
    if (visited[indexFrom]) {
        return false;
    }

    visited[indexFrom] = true;

    if (m_links[indexFrom][indexTo]) {
        return true;
    }

    for (std::size_t i = 0; i < m_links[indexFrom].size(); i++) {
        if (m_links[indexFrom][i]) {
            if (hasPathByIndex(i, indexTo, visited) && m_links[indexFrom][i]) {
                return true;
            }
        }
    }

    return false;
}

std::size_t AbstractGraph::getNodeIndex(std::size_t id) const {
    for (std::size_t nodeId = 0; auto & node : m_nodes) {
        if (node == id) {
            return nodeId;
        }

        nodeId++;
    }

    return std::numeric_limits<std::size_t>::max();
}

std::vector<std::size_t> AbstractGraph::getNeighboursByIndex(std::size_t index) const {
    std::vector<std::size_t> result;

    if (index == std::numeric_limits<std::size_t>::max()) {
        return result;
    }

    for (std::size_t i = 0; i < m_links[index].size(); i++) {
        if (m_links[index][i]) {
            result.emplace_back(i);
        }
    }

    return result;
}

void AbstractGraph::setWeightByIndices(std::size_t indexFrom, std::size_t indexTo, int value) {
    if (indexFrom == std::numeric_limits<std::size_t>::max() ||
        indexTo == std::numeric_limits<std::size_t>::max()) {
        return;
    }

    m_links[indexFrom][indexTo] = value;
}

void Graph::addLink(std::size_t from, std::size_t to) {
    setWeightByIndices(getNodeIndex(from), getNodeIndex(to), 1);
    setWeightByIndices(getNodeIndex(to), getNodeIndex(from), 1);
}

void Graph::removeLink(std::size_t from, std::size_t to) {
    setWeightByIndices(getNodeIndex(from), getNodeIndex(to), 0);
    setWeightByIndices(getNodeIndex(to), getNodeIndex(from), 0);
}

void OrientedGraph::addLink(std::size_t from, std::size_t to) {
    setWeightByIndices(getNodeIndex(from), getNodeIndex(to), 1);
}

void OrientedGraph::removeLink(std::size_t from, std::size_t to) {
    setWeightByIndices(getNodeIndex(from), getNodeIndex(to), 0);
}

void WeightedGraph::addLink(std::size_t from, std::size_t to, int value) {
    setWeightByIndices(getNodeIndex(from), getNodeIndex(to), value);
}

void WeightedGraph::setWeight(std::size_t from, std::size_t to, int value) {
    setWeightByIndices(getNodeIndex(from), getNodeIndex(to), value);
}

void PrintGraph(const AbstractGraph& graph) {
    std::cout << "Nodes:\n";
    for (auto& vertex : graph.m_nodes) {
        std::cout << vertex << " ";
    }

    std::cout << "\n\n";

    std::cout << "Links:\n";
    for (auto& row : graph.m_links) {
        for (auto path : row) {
            std::cout << path << " ";
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}
