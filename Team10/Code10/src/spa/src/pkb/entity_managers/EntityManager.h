#pragma once

#include <unordered_set>
#include <vector>

#include "pkb/interfaces/IReadEntity.h"
#include "pkb/interfaces/IWriteEntity.h"

template <typename T>
class EntityManager : public IWriteEntity<T>,
                      public IReadEntity<T> {
   private:
    std::unordered_set<T> entitiesSet;

   public:
    // only for entities using keys only
    void insertEntity(T key) override {
        entitiesSet.insert(key);
    }

    bool isEmpty() override {
        return entitiesSet.empty();
    }

    bool containsKey(T key) override {
        bool found = false;
        auto element = entitiesSet.find(key);
        if (element != entitiesSet.end()) {
            found = true;
        }
        return found;
    }

    std::unordered_set<T> getAllEntitiesEntries() override {
        return entitiesSet;
    }
    std::vector<T> getAllEntitiesEntriesVector() {
        // Create a vector and reserve space for the elements
        std::vector<T> myVector;
        myVector.reserve(entitiesSet.size());

        // Iterate over the set and add elements to the vector
        for (const T& element : entitiesSet) {
            myVector.push_back(element);
        }
        return myVector;
    }
};