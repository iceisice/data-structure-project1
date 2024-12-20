// DijkstraAlgorithm.cpp

#include "DijkstraAlgorithm.h"

DijkstraAlgorithm::DijkstraAlgorithm(const Graph& graphRef) : graph(graphRef) {}

double DijkstraAlgorithm::calculateTraversalTime(const Edge& edge, const UserPreferences& userPrefs, const std::string& userName, double preferenceWeight) const {
    // 기본 이동 시간: 거리 / 속도
    double baseTime = edge.distance / edge.speed;

    // 혼잡도에 따른 추가 시간: baseTime * congestion
    double congestionFactor = 1.0 + edge.congestion; // 혼잡도가 0.5이면, 1.5배

    // 노드 선호도에 따른 가중치 적용
    double preferenceScore = userPrefs.getPreference(userName, edge.destination);
    double preferenceFactor = 1.0 - (preferenceWeight * preferenceScore);
    if (preferenceFactor < 0.1) preferenceFactor = 0.1; // 최소 가중치 설정

    double traversalTime = baseTime * congestionFactor * preferenceFactor;

    return traversalTime;
}

std::unordered_map<std::string, double> DijkstraAlgorithm::computeShortestPaths(
    const std::string& startNode,
    const std::string& userName,
    const UserPreferences& userPrefs,
    double preferenceWeight) const {

    std::unordered_map<std::string, double> distances;
    std::priority_queue<std::pair<double, std::string>,
        std::vector<std::pair<double, std::string> >,
        std::greater<std::pair<double, std::string> > > pq;

    // 시작 노드 초기화
    distances[startNode] = 0.0;
    pq.push({ 0.0, startNode });

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        std::string currentNode = pq.top().second;
        pq.pop();

        // 이미 더 짧은 경로가 발견된 경우 무시
        if (currentDist > distances[currentNode]) continue;

        // 인접 노드 탐색
        const std::vector<Edge>& neighbors = graph.getAdjacencyList().at(currentNode);
        for (const auto& edge : neighbors) {
            double traversalTime = calculateTraversalTime(edge, userPrefs, userName, preferenceWeight);
            double newDist = currentDist + traversalTime;

            // 더 짧은 경로 발견 시 업데이트
            if (distances.find(edge.destination) == distances.end() || newDist < distances[edge.destination]) {
                distances[edge.destination] = newDist;
                pq.push({ newDist, edge.destination });
            }
        }
    }

    return distances;
}
