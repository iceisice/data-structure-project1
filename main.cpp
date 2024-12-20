// main.cpp

#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include "Graph.h"
#include "DijkstraAlgorithm.h"
#include "UserPreferences.h"

struct User {
    std::string name;
    std::string location;
    double preferenceWeight = 0.5;
    std::unordered_map<std::string, double> distances;
};

int main() {
    Graph graph;
    UserPreferences userPrefs;

    int nodeCount, edgeCount;
    std::cout << "노드(지역)의 개수를 입력하세요: ";
    std::cin >> nodeCount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::string> nodeList;

    for (int i = 0; i < nodeCount; ++i) {
        std::string nodeName;
        bool validNode = false;

        while (!validNode) {
            std::cout << i + 1 << "번째 노드의 이름을 입력하세요: ";
            std::getline(std::cin, nodeName);

            if (graph.hasNode(nodeName)) {
                std::cout << "이미 존재하는 노드 이름입니다. 다른 이름을 입력해주세요.\n";
            }
            else {
                validNode = true;
                graph.addNode(nodeName);
                nodeList.push_back(nodeName);
            }
        }
    }

    std::cout << "간선(도로)의 개수를 입력하세요: ";
    std::cin >> edgeCount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    struct EdgeInput {
        std::string source;
        std::string destination;
        double distance;
        double speed;
        double congestion;
    };
    std::vector<EdgeInput> edges;

    for (int i = 0; i < edgeCount; ++i) {
        std::string source, destination;
        double distance, speed, congestion;
        bool validEdge = false;

        while (!validEdge) {
            std::cout << i + 1 << "번째 간선의 출발 노드, 도착 노드, 거리(km), 속도(km/h), 초기 혼잡도(0.0~1.0)를 입력하세요: ";
            std::cin >> source >> destination >> distance >> speed >> congestion;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!graph.hasNode(source) || !graph.hasNode(destination)) {
                std::cout << "입력된 노드가 그래프에 존재하지 않습니다. 다시 입력해주세요.\n";
                continue;
            }

            if (distance <= 0 || speed <= 0) {
                std::cout << "거리와 속도는 0보다 큰 값이어야 합니다. 다시 입력해주세요.\n";
                continue;
            }

            if (congestion < 0.0 || congestion > 1.0) {
                std::cout << "혼잡도는 0.0에서 1.0 사이의 값이어야 합니다. 다시 입력해주세요.\n";
                continue;
            }

            validEdge = true;
            edges.push_back({ source, destination, distance, speed, congestion });
        }
    }

    for (size_t i = 0; i < edges.size(); ++i) {
        const EdgeInput& edge = edges[i];
        graph.addEdge(edge.source, edge.destination, edge.distance, edge.speed, edge.congestion);
    }

    double segmentLength = 0.5;
    graph.subdivideEdges(segmentLength);

    userPrefs.setVirtualNodePreferences(graph.getAdjacencyList());

    std::vector<User> users;

    while (true) {
        User user;
        std::cout << "\n새로운 사용자를 추가 (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'n' || choice == 'N') {
            break;
        }

        while (true) {
            std::cout << "사용자의 이름은?: ";
            std::getline(std::cin, user.name);
            std::cout << user.name << "의 위치는?: ";
            std::getline(std::cin, user.location);

            if (!graph.hasNode(user.location)) {
                std::cout << "입력된 위치가 그래프에 존재하지 않습니다.다시 입력해주세요.\n";
            }
            else {
                break;
            }
        }

        std::cout << user.name << "의 각 노드에 대한 선호도를 입력하세요 (0.0 ~ 1.0):\n";
        for (size_t i = 0; i < nodeList.size(); ++i) {
            const std::string& node = nodeList[i];
            double score;
            while (true) {
                std::cout << node << ": ";
                std::cin >> score;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "숫자를 입력해주세요.\n";
                    continue;
                }
                if (score < 0.0 || score > 1.0) {
                    std::cout << "선호도는 0.0에서 1.0 사이의 값이어야 합니다. 다시 입력해주세요.\n";
                }
                else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
            }
            userPrefs.setPreference(user.name, node, score);
        }

        std::cout << user.name << "의 선호도 가중치 값을 입력하세요 (0.5로 했을 때 가장 이상적): ";
        while (true) {
            std::cin >> user.preferenceWeight;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "숫자를 입력해주세요.\n";
                continue;
            }
            else {
                if (user.preferenceWeight < 0.0) {
                    std::cout << "선호도 가중치는 0 이상이어야 합니다. 다시 입력해주세요.\n";
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }

        users.push_back(user);
    }

    DijkstraAlgorithm dijkstra(graph);
    for (size_t i = 0; i < users.size(); ++i) {
        User& user = users[i];
        user.distances = dijkstra.computeShortestPaths(
            user.location,
            user.name,
            userPrefs,
            user.preferenceWeight);
    }

    const std::unordered_map<std::string, std::vector<Edge> >& adjacencyList = graph.getAdjacencyList();
    std::unordered_map<std::string, double> totalDistances;
    for (const auto& it : adjacencyList) {
        const std::string& node = it.first;

        if (node.find('_') == std::string::npos) {
            continue;
        }

        double totalTime = 0.0;
        bool reachable = true;
        for (size_t i = 0; i < users.size(); ++i) {
            const User& user = users[i];
            std::unordered_map<std::string, double>::const_iterator distIt = user.distances.find(node);
            if (distIt != user.distances.end()) {
                totalTime += distIt->second;
            }
            else {
                reachable = false;
                break;
            }
        }
        if (reachable) {
            totalDistances[node] = totalTime;
        }
    }

    if (totalDistances.empty()) {
        std::cout << "\n모든 사용자가 도달할 수 있는 약속 장소가 없습니다.\n";
    }
    else {
        std::string recommendedNode;
        double minTotalTime = std::numeric_limits<double>::max();
        for (const auto& it : totalDistances) {
            if (it.second < minTotalTime) {
                minTotalTime = it.second;
                recommendedNode = it.first;
            }
        }

        for (size_t i = 0; i < users.size(); ++i) {
            const User& user = users[i];
            std::cout << "\n" << user.name << "에서 각 노드까지의 예상 시간:\n";
            for (const auto& distIt : user.distances) {
                const std::string& node = distIt.first;
                double time = distIt.second;
                std::cout << node << ": " << time << " 시간\n";
            }
        }

        std::cout << "\n추천 약속 장소: ";
        if (recommendedNode.find('_') != std::string::npos) {
            size_t firstUnderscore = recommendedNode.find('_');
            size_t secondUnderscore = recommendedNode.find('_', firstUnderscore + 1);

            std::string sourceNode = recommendedNode.substr(0, firstUnderscore);
            std::string destinationNode = recommendedNode.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);
            int segmentIndex = std::stoi(recommendedNode.substr(secondUnderscore + 1));

            double position = segmentIndex * segmentLength;

            std::cout << sourceNode << "에서 " << destinationNode << " 방향으로 " << position << "km 지점\n";
        }
        else {
            std::cout << recommendedNode << "\n";
        }
        std::cout << "모든 사용자로부터의 총 예상 소요 시간: " << minTotalTime << " 시간\n";
    }

    std::cout << "\n프로그램을 종료하려면 엔터를 누르세요.";
    std::cin.get();
    return 0;
}
