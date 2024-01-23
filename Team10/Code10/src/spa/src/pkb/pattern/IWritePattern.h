#pragma once

#include <memory>
#include <string>

template <typename T, typename U>
class IWritePattern {
   public:
    virtual void insertPattern(T left, U right, int stmtNum) = 0;
};