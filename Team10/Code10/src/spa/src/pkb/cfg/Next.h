#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

class Next {
    //ai-gen start (copilot, 2)
   private:
    std::unordered_map<int, std::vector<int>> cache;
    mutable std::unordered_map<int, std::vector<int>> reverseGraphCache;
    mutable bool isCacheValid = false;
    std::unordered_map<int, std::vector<int>> reverseCache;
    std::unordered_map<int, std::vector<int>> rsMap;

    void dfs(const std::unordered_map<int, std::vector<int>>& graph,
             int node,
             std::unordered_set<int>& visited,
             std::vector<int>& result,
             int startNode) {  // Add startNode as a parameter to keep track of it.
        if (visited.count(node) == 0) {
            if (node != startNode || (node == startNode && visited.size() >= 1)) {
                visited.insert(node);
                // Add node to result if it's not the startNode or if startNode is reachable from a different node.
                result.push_back(node);
            }

            // Check if the node is in the graph map
            if (graph.find(node) != graph.end()) {
                for (int neighbor : graph.at(node)) {
                    dfs(graph, neighbor, visited, result, startNode);
                }
            }
        }
    }

    std::unordered_map<int, std::vector<int>> reversingGraph(const std::unordered_map<int,
                                                                                      std::vector<int>>& originalGraph) {
        std::unordered_map<int, std::vector<int>> reversedGraph;

        if (isCacheValid) {
            return reverseGraphCache;
        }

        for (const auto& entry : originalGraph) {
            int node = entry.first;
            const std::vector<int>& neighbors = entry.second;

            for (int neighbor : neighbors) {
                reversedGraph[neighbor].push_back(node);
            }
        }
        reverseGraphCache = reversedGraph;
        isCacheValid = true;
        return reversedGraph;
    }

   public:
    Next() {}

    void insertNext(int key, std::vector<int> value) {
        auto it = rsMap.find(key);  // Check if the key exists in the map

        if (it != rsMap.end()) {  // Key exists
            auto& existingValue = it->second;
            auto& newValue = value;  // Add only those elements that don't exist in the vector
            for (const auto& element : newValue) {
                if (std::find(existingValue.begin(), existingValue.end(), element) == existingValue.end()) {
                    existingValue.push_back(element);
                }
            }
        } else {  // Key does not exist
            rsMap.emplace(key, value);
        }
    }

    std::vector<int> getNextS2(int stmtNum) {
        if (isCacheValid) {
            auto it = reverseGraphCache.find(stmtNum);  // Use find to safely check for key
            if (it != reverseGraphCache.end() && !it->second.empty()) {
                // If the key exists and the vector is not empty, return the vector
                return it->second;
            } else {
                // If the key does not exist or the vector is empty, return an empty vector
                return {};
            }
        } else {
            // If the cache is not valid, call the function to reverse the graph
            std::unordered_map<int, std::vector<int>> reversed = reversingGraph(rsMap);
            // Now we can return the result safely using the same method as above
            auto it = reversed.find(stmtNum);
            if (it != reversed.end() && !it->second.empty()) {
                return it->second;
            } else {
                return {};
            }
        }
    }

    std::vector<int> getNext(const int& key) const {
        auto it = rsMap.find(key);  // Try to find the key in the map

        if (it != rsMap.end()) {
            // Key found, return the associated value
            return it->second;
        } else {
            // Key not found, handle the error accordingly:
            //          throw std::out_of_range("Key not found in rsMap");
            return std::vector<int>{};
        }
    }

    std::vector<int> findAllReachableNodes(int startNode, bool isReverse) {
        const std::unordered_map<int, std::vector<int>>& graphToUse = isReverse ? reversingGraph(rsMap) : rsMap;
        std::unordered_map<int, std::vector<int>>& cacheToUse = isReverse ? reverseCache : cache;

        auto it = cacheToUse.find(startNode);  // Check if the key exists in the cache

        if (it != cacheToUse.end()) {
            return it->second;
        }

        std::vector<int> result;
        std::unordered_set<int> visited;

        if (graphToUse.size() > 1) {
            dfs(graphToUse, startNode, visited, result, startNode);  // Pass startNode as an additional argument.
        } else {
            result = {};
        }

        cacheToUse.emplace(startNode, result);
        return result;
    }

    bool notEmpty() const {
        return !rsMap.empty();
    }

    bool hasNextStar() {
        for (const auto& kv : rsMap) {
            for (int val : kv.second) {
                if (rsMap.find(val) != rsMap.end()) {
                    // Found an integer in the vector that is also a key in the map.
                    return true;
                }
            }
        }
        // No integer found in any vector that is also a key in the map.
        return false;
    }

    void clearCache() {
        cache.clear();
        reverseGraphCache.clear();
        reverseCache.clear();
        isCacheValid = false;
    }
    // ai-gen end
};