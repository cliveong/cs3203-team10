#pragma once

#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

class Follow : public RelationshipManager<int, std::vector<int>> {
   public:
    Follow() {}
    // ai-gen start (chatgpt, 2)
    std::vector<int> findKeysWithInt(int valueToFind, bool isStar) const {
        std::vector<int> resultKeys;

        // Check the cache first.
        if (isCached(valueToFind, resultKeys, isStar)) {
            return resultKeys;
        }

        const std::unordered_map<int, std::vector<int>>& rsMap = getRsMap();

        // Iterate through the keys (from valueToFind-1 to 1)
        for (int key = valueToFind - 1; key >= 1; --key) {
            // Check if the key exists in the rsMap
            auto it = rsMap.find(key);

            if (it != rsMap.end()) {
                const std::vector<int>& values = it->second;

                // Check if the vector of values contains the desired value
                if (std::find(values.begin(), values.end(), valueToFind) != values.end()) {
                    resultKeys.push_back(key);

                    // If isStar is false, only the first key is needed.
                    if (!isStar) {
                        break;
                    }
                }
            }
        }

        // Update the cache with the results before returning them.
        updateCache(valueToFind, resultKeys, isStar);

        return resultKeys;
    }

    bool hasFollowStar() {
        const std::unordered_map<int, std::vector<int>>& rsMap = getRsMap();
        for (const auto& pair : rsMap) {
            if (pair.second.size() > 1) {
                return true;  // Found a vector with more than one integer.
            }
        }
        return false;  // No vector with more than one integer was found.
    }
    // ai-gen end
};