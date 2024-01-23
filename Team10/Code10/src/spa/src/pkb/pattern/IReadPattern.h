#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

template <typename T, typename U>
class IReadPattern {
   public:
    virtual bool isEmptyLhsMap() = 0;
    virtual bool isEmptyRhsVector() = 0;
    virtual bool containsLhsExpr(T left) = 0;
    virtual bool containsSubTreeRhs(const U &node, const U &subTree) = 0;
    virtual std::vector<int> getLhsValues(T key) = 0;
    virtual std::vector<int> getRhsValues(U &rhsKey, bool isFullMatch) = 0;
    virtual std::vector<int> getRhsIndexes(const U &key, const bool &isFullMatch) = 0;
    virtual std::vector<int> getAllMatch(const T &left, const U &right, bool isFullMatch) = 0;
    virtual int getStmtFromIndexMap(int index) = 0;
};