// UserPreferences.cpp

#include "UserPreferences.h"

void UserPreferences::setPreference(const std::string& user, const std::string& node, double score) {
    preferences[user][node] = score;
}

double UserPreferences::getPreference(const std::string& user, const std::string& node) const {
    auto userIt = preferences.find(user);
    if (userIt != preferences.end()) {
        auto nodeIt = userIt->second.find(node);
        if (nodeIt != userIt->second.end()) {
            return nodeIt->second;
        }
    }
    return 0.0;
}

void UserPreferences::setVirtualNodePreferences(const std::unordered_map<std::string, std::vector<Edge> >& adjacencyList) {
    for (auto& userPair : preferences) {
        const std::string& userName = userPair.first;
        std::map<std::string, double>& nodePrefs = userPair.second;

        for (const auto& adjPair : adjacencyList) {
            const std::string& node = adjPair.first;

            size_t firstUnderscore = node.find('_');
            size_t secondUnderscore = node.find('_', firstUnderscore + 1);
            if (firstUnderscore != std::string::npos && secondUnderscore != std::string::npos) {
                std::string sourceNode = node.substr(0, firstUnderscore);
                std::string destinationNode = node.substr(firstUnderscore + 1, secondUnderscore - firstUnderscore - 1);

                double sourcePref = getPreference(userName, sourceNode);
                double destPref = getPreference(userName, destinationNode);
                double virtualNodePref = (sourcePref + destPref) / 2.0;

                nodePrefs[node] = virtualNodePref;
            }
        }
    }
}
