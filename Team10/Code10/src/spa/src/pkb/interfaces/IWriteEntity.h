#pragma once

template <typename T>
class IWriteEntity {
   public:
    virtual void insertEntity(T key) = 0;
};