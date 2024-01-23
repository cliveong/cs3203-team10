#pragma once

#include <unordered_set>

template <typename T>
class IReadEntity {
   public:
    virtual bool isEmpty() = 0;
    virtual bool containsKey(T entity) = 0;
    virtual std::unordered_set<T> getAllEntitiesEntries() = 0;
};