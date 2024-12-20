// DijkstraAlgorithm.cpp

#include "DijkstraAlgorithm.h"

DijkstraAlgorithm::DijkstraAlgorithm(const Graph& graphRef) : graph(graphRef) {}

double DijkstraAlgorithm::calculateTraversalTime(const Edge& edge, const UserPreferences& userPrefs, const std::string& userName, double preferenceWeight) const {
    // �⺻ �̵� �ð�: �Ÿ� / �ӵ�
    double baseTime = edge.distance / edge.speed;

    // ȥ�⵵�� ���� �߰� �ð�: baseTime * congestion
    double congestionFactor = 1.0 + edge.congestion; // ȥ�⵵�� 0.5�̸�, 1.5��

    // ��� ��ȣ���� ���� ����ġ ����
    double preferenceScore = userPrefs.getPreference(userName, edge.destination);
    double preferenceFactor = 1.0 - (preferenceWeight * preferenceScore);
    if (preferenceFactor < 0.1) preferenceFactor = 0.1; // �ּ� ����ġ ����

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

    // ���� ��� �ʱ�ȭ
    distances[startNode] = 0.0;
    pq.push({ 0.0, startNode });

    while (!pq.empty()) {
        double currentDist = pq.top().first;
        std::string currentNode = pq.top().second;
        pq.pop();

        // �̹� �� ª�� ��ΰ� �߰ߵ� ��� ����
        if (currentDist > distances[currentNode]) continue;

        // ���� ��� Ž��
        const std::vector<Edge>& neighbors = graph.getAdjacencyList().at(currentNode);
        for (const auto& edge : neighbors) {
            double traversalTime = calculateTraversalTime(edge, userPrefs, userName, preferenceWeight);
            double newDist = currentDist + traversalTime;

            // �� ª�� ��� �߰� �� ������Ʈ
            if (distances.find(edge.destination) == distances.end() || newDist < distances[edge.destination]) {
                distances[edge.destination] = newDist;
                pq.push({ newDist, edge.destination });
            }
        }
    }

    return distances;
}
