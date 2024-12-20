#pragma once
// UserPreferences.h

#pragma once
#include <unordered_map>
#include <string>
#include <map>
#include <vector>
#include "Graph.h" // Edge ����ü�� ����ϱ� ���� �ʿ�

class UserPreferences {
public:
    void setPreference(const std::string& user, const std::string& node, double score);

    double getPreference(const std::string& user, const std::string& node) const;

    void setVirtualNodePreferences(const std::unordered_map<std::string, std::vector<Edge> >& adjacencyList);

private:
    std::unordered_map<std::string, std::map<std::string, double> > preferences;
};
