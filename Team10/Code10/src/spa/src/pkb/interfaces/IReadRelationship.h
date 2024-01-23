#pragma once

#include <unordered_map>

template <typename T, typename U>
class IReadRelationship {
   public:
    // Define a virtual method to retrieve a value from the unordered map by key
    virtual U getRelationship(const T& key) const = 0;

    virtual std::vector<T> getAllKeys() const = 0;

    virtual bool hasKeyValues() const = 0;

    virtual std::vector<T> findKeysWithValue(const typename U::value_type& valueToFind) const = 0;
};