#pragma once

template <typename T, typename U>
class IWriteRelationship {
   public:
    // Declare a pure virtual function to insert a relationship between a key (T) and a value (U)
    virtual void insertRelationship(T key, U value) = 0;
};
