#pragma once
// DijkstraAlgorithm.h

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include "Graph.h"
#include "UserPreferences.h"

class DijkstraAlgorithm {
public:
    DijkstraAlgorithm(const Graph& graph);

    std::unordered_map<std::string, double> computeShortestPaths(
        const std::string& startNode,
        const std::string& userName,
        const UserPreferences& userPrefs,
        double preferenceWeight) const;

private:
    const Graph& graph;

    double calculateTraversalTime(const Edge& edge, const UserPreferences& userPrefs, const std::string& userName, double preferenceWeight) const;
};
