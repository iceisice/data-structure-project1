#pragma once
// Graph.h

#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// ���� ����ü ����
struct Edge {
    std::string source;
    std::string destination;
    double distance;    // �Ÿ� (km)
    double speed;       // �ӵ� (km/h)
    double congestion;  // ȥ�⵵ (0.0 ~ 1.0)
};

// �׷��� Ŭ���� ����
class Graph {
public:
    // ��� �߰�
    void addNode(const std::string& nodeName);

    // ���� �߰�
    void addEdge(const std::string& source, const std::string& destination, double distance, double speed, double congestion);

    // ��� ���� ���� Ȯ��
    bool hasNode(const std::string& nodeName) const;

    // ���� ���� �� ���� ��� ����
    void subdivideEdges(double segmentLength);

    // ���� ����Ʈ ��ȯ
    const std::unordered_map<std::string, std::vector<Edge> >& getAdjacencyList() const;

private:
    // ���� ����Ʈ (��� �̸� -> ���� ����Ʈ)
    std::unordered_map<std::string, std::vector<Edge> > adjacencyList;
};
