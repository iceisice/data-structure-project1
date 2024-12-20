// Graph.cpp

#include "Graph.h"

void Graph::addNode(const std::string& nodeName) {
    // 노드가 이미 존재하면 아무것도 하지 않음
    if (adjacencyList.find(nodeName) != adjacencyList.end()) {
        return;
    }
    adjacencyList[nodeName] = std::vector<Edge>();
}

void Graph::addEdge(const std::string& source, const std::string& destination, double distance, double speed, double congestion) {
    Edge edge = { source, destination, distance, speed, congestion };
    adjacencyList[source].push_back(edge);
}

bool Graph::hasNode(const std::string& nodeName) const {
    return adjacencyList.find(nodeName) != adjacencyList.end();
}

void Graph::subdivideEdges(double segmentLength) {
    std::vector<Edge> originalEdges;
    for (const auto& pair : adjacencyList) {
        for (const auto& edge : pair.second) {
            originalEdges.push_back(edge);
        }
    }

    for (auto& pair : adjacencyList) {
        pair.second.clear();
    }

    for (const auto& edge : originalEdges) {
        double remainingDistance = edge.distance;
        std::string currentNode = edge.source;
        int segmentIndex = 1;

        while (remainingDistance > segmentLength) {
            std::string virtualNode = edge.source + "_" + edge.destination + "_" + std::to_string(segmentIndex);
            segmentIndex++;

            addNode(virtualNode);

            addEdge(currentNode, virtualNode, segmentLength, edge.speed, edge.congestion);

            currentNode = virtualNode;
            remainingDistance -= segmentLength;
        }

        if (remainingDistance > 0) {
            addEdge(currentNode, edge.destination, remainingDistance, edge.speed, edge.congestion);
        }
    }
}



const std::unordered_map<std::string, std::vector<Edge> >& Graph::getAdjacencyList() const {
    return adjacencyList;
}
