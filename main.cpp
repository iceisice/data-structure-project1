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
    std::cout << "���(����)�� ������ �Է��ϼ���: ";
    std::cin >> nodeCount;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<std::string> nodeList;

    for (int i = 0; i < nodeCount; ++i) {
        std::string nodeName;
        bool validNode = false;

        while (!validNode) {
            std::cout << i + 1 << "��° ����� �̸��� �Է��ϼ���: ";
            std::getline(std::cin, nodeName);

            if (graph.hasNode(nodeName)) {
                std::cout << "�̹� �����ϴ� ��� �̸��Դϴ�. �ٸ� �̸��� �Է����ּ���.\n";
            }
            else {
                validNode = true;
                graph.addNode(nodeName);
                nodeList.push_back(nodeName);
            }
        }
    }

    std::cout << "����(����)�� ������ �Է��ϼ���: ";
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
            std::cout << i + 1 << "��° ������ ��� ���, ���� ���, �Ÿ�(km), �ӵ�(km/h), �ʱ� ȥ�⵵(0.0~1.0)�� �Է��ϼ���: ";
            std::cin >> source >> destination >> distance >> speed >> congestion;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (!graph.hasNode(source) || !graph.hasNode(destination)) {
                std::cout << "�Էµ� ��尡 �׷����� �������� �ʽ��ϴ�. �ٽ� �Է����ּ���.\n";
                continue;
            }

            if (distance <= 0 || speed <= 0) {
                std::cout << "�Ÿ��� �ӵ��� 0���� ū ���̾�� �մϴ�. �ٽ� �Է����ּ���.\n";
                continue;
            }

            if (congestion < 0.0 || congestion > 1.0) {
                std::cout << "ȥ�⵵�� 0.0���� 1.0 ������ ���̾�� �մϴ�. �ٽ� �Է����ּ���.\n";
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
        std::cout << "\n���ο� ����ڸ� �߰� (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (choice == 'n' || choice == 'N') {
            break;
        }

        while (true) {
            std::cout << "������� �̸���?: ";
            std::getline(std::cin, user.name);
            std::cout << user.name << "�� ��ġ��?: ";
            std::getline(std::cin, user.location);

            if (!graph.hasNode(user.location)) {
                std::cout << "�Էµ� ��ġ�� �׷����� �������� �ʽ��ϴ�.�ٽ� �Է����ּ���.\n";
            }
            else {
                break;
            }
        }

        std::cout << user.name << "�� �� ��忡 ���� ��ȣ���� �Է��ϼ��� (0.0 ~ 1.0):\n";
        for (size_t i = 0; i < nodeList.size(); ++i) {
            const std::string& node = nodeList[i];
            double score;
            while (true) {
                std::cout << node << ": ";
                std::cin >> score;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "���ڸ� �Է����ּ���.\n";
                    continue;
                }
                if (score < 0.0 || score > 1.0) {
                    std::cout << "��ȣ���� 0.0���� 1.0 ������ ���̾�� �մϴ�. �ٽ� �Է����ּ���.\n";
                }
                else {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
            }
            userPrefs.setPreference(user.name, node, score);
        }

        std::cout << user.name << "�� ��ȣ�� ����ġ ���� �Է��ϼ��� (0.5�� ���� �� ���� �̻���): ";
        while (true) {
            std::cin >> user.preferenceWeight;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "���ڸ� �Է����ּ���.\n";
                continue;
            }
            else {
                if (user.preferenceWeight < 0.0) {
                    std::cout << "��ȣ�� ����ġ�� 0 �̻��̾�� �մϴ�. �ٽ� �Է����ּ���.\n";
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
        std::cout << "\n��� ����ڰ� ������ �� �ִ� ��� ��Ұ� �����ϴ�.\n";
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
            std::cout << "\n" << user.name << "���� �� �������� ���� �ð�:\n";
            for (const auto& distIt : user.distances) {
                const std::string& node = distIt.first;
                double time = distIt.second;
                std::cout << node << ": " << time << " �ð�\n";
            }
        }

        std::cout << "\n��õ ��� ���: ";
        if (recommendedNode.find('_') != std::string::npos) {
            size_t firstUnderscore = recommendedNode.find('_');
            size_t secondUnderscore = recommendedNode.find('_', firstUnderscore + 1);

            std::string sourceNode = recommendedNode.substr(0, firstUnderscore);
            std::string destinationNode = recommendedNode.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);
            int segmentIndex = std::stoi(recommendedNode.substr(secondUnderscore + 1));

            double position = segmentIndex * segmentLength;

            std::cout << sourceNode << "���� " << destinationNode << " �������� " << position << "km ����\n";
        }
        else {
            std::cout << recommendedNode << "\n";
        }
        std::cout << "��� ����ڷκ����� �� ���� �ҿ� �ð�: " << minTotalTime << " �ð�\n";
    }

    std::cout << "\n���α׷��� �����Ϸ��� ���͸� ��������.";
    std::cin.get();
    return 0;
}
