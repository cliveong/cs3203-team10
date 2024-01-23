#pragma once

#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

class ParentStar : public RelationshipManager<int, std::vector<int>> {
   public:
    ParentStar() {}
    // ai-gen start (chatgpt, 2)
    std::vector<int> findKeysWithInt(int valueToFind) const {
        // First, attempt to retrieve the result from the cache.
        std::vector<int> cachedResult;
        if (isCached(valueToFind, cachedResult, false)) {
            return cachedResult;
        }

        // If the result is not in the cache, proceed to search.
        std::vector<int> resultKeys;
        const std::unordered_map<int, std::vector<int>>& rsMap = getRsMap();

        // Iterate through the keys (from 1 to the maximum key value)
        for (int key = 1; key <= valueToFind; ++key) {  // Fix: Changed '<' to '<=' if you want to include the valueToFind key.
            // Check if the key exists in the rsMap
            auto it = rsMap.find(key);

            if (it != rsMap.end()) {
                const std::vector<int>& values = it->second;

                // Check if the vector of values contains the desired value
                if (std::find(values.begin(), values.end(), valueToFind) != values.end()) {
                    resultKeys.push_back(key);
                }
            }
        }

        // Update the cache with the newly found result.
        updateCache(valueToFind, resultKeys, false);

        return resultKeys;
    }
    // ai-gen end
};