#pragma once
// Graph.h

#pragma once
#include <string>
#include <vector>
#include <unordered_map>

// 간선 구조체 정의
struct Edge {
    std::string source;
    std::string destination;
    double distance;    // 거리 (km)
    double speed;       // 속도 (km/h)
    double congestion;  // 혼잡도 (0.0 ~ 1.0)
};

// 그래프 클래스 정의
class Graph {
public:
    // 노드 추가
    void addNode(const std::string& nodeName);

    // 간선 추가
    void addEdge(const std::string& source, const std::string& destination, double distance, double speed, double congestion);

    // 노드 존재 여부 확인
    bool hasNode(const std::string& nodeName) const;

    // 간선 분할 및 가상 노드 생성
    void subdivideEdges(double segmentLength);

    // 인접 리스트 반환
    const std::unordered_map<std::string, std::vector<Edge> >& getAdjacencyList() const;

private:
    // 인접 리스트 (노드 이름 -> 간선 리스트)
    std::unordered_map<std::string, std::vector<Edge> > adjacencyList;
};
