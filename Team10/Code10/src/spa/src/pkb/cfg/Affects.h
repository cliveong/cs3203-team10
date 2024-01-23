#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>

class Affects {
   private:
    std::unordered_map<int, std::vector<int>> cache;
    std::unordered_map<int, std::vector<int>> reverseCache;
    std::unordered_map<int, std::unordered_set<std::string>> usesCache;
    std::unordered_map<std::string, std::unordered_set<std::string>> modifiesProcCache;

   public:
    Affects() = default;

    void insertAffects(const int& firstParam, const std::vector<int>& secondParam, bool isReverse) {
        std::unordered_map<int, std::vector<int>>& targetMap = isReverse ? reverseCache : cache;

        if (!targetMap.count(firstParam)) {
            auto res = targetMap.emplace(firstParam, secondParam);
        } else {
            auto& existingVector = targetMap[firstParam];
            for (const int& value : secondParam) {
                if (std::find(existingVector.begin(), existingVector.end(), value) == existingVector.end()) {
                    existingVector.push_back(value);
                }
            }
        }
    }

    void insertUsesCache(const int& stmt, const std::string& v) {
        // Check if the key already exists in the map
        auto it = usesCache.find(stmt);

        if (it == usesCache.end()) {
            // If the key doesn't exist, create a new entry
            std::unordered_set<std::string> newSet;
            newSet.insert(v);
            usesCache[stmt] = newSet;
        } else {
            // If the key already exists, insert the value into the set
            it->second.insert(v);
        }
    }

    void insertModifiesProcCache(const std::string& proc, const std::string& v) {
        // Check if the key already exists in the map
        auto it = modifiesProcCache.find(proc);

        if (it == modifiesProcCache.end()) {
            // If the key doesn't exist, create a new entry
            std::unordered_set<std::string> newSet;
            newSet.insert(v);
            modifiesProcCache[proc] = newSet;
        } else {
            // If the key already exists, insert the value into the set
            it->second.insert(v);
        }
    }

    bool containsUses(const int& stmt, const std::string& v) {
        auto key = usesCache.find(stmt);
        if (key != usesCache.end()) {
            auto res = key->second;
            return res.count(v);
        } else {
            return false;
        }
    }

    bool containsModifiesProc(const std::string& proc, const std::string& v) {
        auto key = modifiesProcCache.find(proc);
        if (key != modifiesProcCache.end()) {
            auto res = key->second;
            return res.count(v);
        } else {
            return false;
        }
    }

    std::vector<int> getAffectedStmt(const int& key, const bool isReverse) {
        std::unordered_map<int, std::vector<int>> target = isReverse ? reverseCache : cache;
        auto it = target.find(key);
        if (it != target.end()) {
            return it->second;
        } else {
            return std::vector<int>{};
        }
    }

    const std::unordered_map<int, std::vector<int>>& getCache() const {
        return cache;
    }
    const std::unordered_map<int, std::vector<int>>& getReverseCache() const {
        return reverseCache;
    }

    void clearCache() {
        cache.clear();
        reverseCache.clear();
        modifiesProcCache.clear();
        usesCache.clear();
    }
};