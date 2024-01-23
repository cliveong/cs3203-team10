#pragma once

#include <unordered_map>

#include "../interfaces/IReadRelationship.h"
#include "../interfaces/IWriteRelationship.h"

template <typename T, typename U>
class RelationshipManager : public IWriteRelationship<T, U>,
                            public IReadRelationship<T, U> {
   private:
    std::unordered_map<T, U> rsMap;
    mutable std::unordered_map<typename U::value_type, std::vector<T>> cache;
    mutable std::unordered_map<typename U::value_type, std::vector<T>> cacheStar;

   protected:
    const std::unordered_map<T, U>& getRsMap() const {
        return rsMap;
    }

    // ai-gen start (copilot, 2)
    bool isCached(const typename U::value_type& value, std::vector<T>& keys, bool isStar) const {
        std::unordered_map<typename U::value_type, std::vector<T>>& cacheToUse = isStar ? cacheStar : cache;
        auto it = cacheToUse.find(value);
        if (it != cacheToUse.end()) {
            keys = it->second;
            return true;
        }
        return false;
    }

    void updateCache(const typename U::value_type& value, const std::vector<T>& keys, bool isStar) const {
        std::unordered_map<typename U::value_type, std::vector<T>>& cacheToUse = isStar ? cacheStar : cache;
        cacheToUse[value] = keys;
    }
    // ai-gen end

   public:
    // ai-gen start (chatgpt, 2)
    void insertRelationship(T key, U value) override {
        auto it = rsMap.find(key);  // Check if the key exists in the map

        if (it != rsMap.end()) {  // Key exists
            if (std::is_same<U, std::vector<typename U::value_type>>::value) {
                // Check if the value type is std::vector
                auto& existingValue = it->second;
                auto& newValue = value;

                // Add only those elements that don't exist in the vector
                for (const auto& element : newValue) {
                    if (std::find(existingValue.begin(), existingValue.end(), element) == existingValue.end()) {
                        existingValue.push_back(element);
                    }
                }
            } else {
                it->second = value;
            }
        } else {  // Key does not exist
            rsMap.emplace(key, value);
        }
    }

    // Implement the getNext method from IReadRelationship
    U getRelationship(const T& key) const override {
        auto it = rsMap.find(key);  // Try to find the key in the map

        if (it != rsMap.end()) {
            // Key found, return the associated value
            return it->second;
        } else {
            // Key not found, handle the error accordingly:
            throw std::out_of_range("Key not found in rsMap");
        }
    }

    std::vector<T> getAllKeys() const override {
        std::vector<T> allKeys;
        for (const auto& pair : rsMap) {
            allKeys.push_back(pair.first);
        }
        return allKeys;
    }

    bool hasKeyValues() const override {
        return !rsMap.empty();
    }

    std::vector<T> findKeysWithValue(const typename U::value_type& valueToFind) const {
        // First, check the cache.
        auto cacheIt = cache.find(valueToFind);
        if (cacheIt != cache.end()) {
            // If found in cache, return the cached result.
            return cacheIt->second;
        }

        // If not found in cache, then proceed to search.
        std::vector<T> resultKeys;
        for (const auto& entry : rsMap) {
            const U& values = entry.second;
            if (std::find(values.begin(), values.end(), valueToFind) != values.end()) {
                resultKeys.push_back(entry.first);
            }
        }

        // Add the result to the cache before returning.
        cache[valueToFind] = resultKeys;
        return resultKeys;
    }

    void clearCache() const {
        cache.clear();
        cacheStar.clear();
    }
    // ai-gen start (chatgpt, 2)
};