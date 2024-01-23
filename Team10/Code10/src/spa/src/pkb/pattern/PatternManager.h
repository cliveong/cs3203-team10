#pragma once

#include "IReadPattern.h"
#include "IWritePattern.h"
#include "unordered_map"

template <typename T, typename U>
class PatternManager : public IWritePattern<T, U>, public IReadPattern<T, U> {
   private:
    std::unordered_map<T, std::vector<int>> lhsMap;
    std::vector<U> rhsVector;
    std::unordered_map<int, int> indexStmtMap;

   public:
    bool isEmptyLhsMap() {
        return lhsMap.empty();
    }

    bool isEmptyRhsVector() {
        return rhsVector.empty();
    }

    bool containsLhsExpr(T left) {
        return lhsMap.find(left) != lhsMap.end();
    }

    std::vector<int> getLhsValues(T key) {
        return lhsMap[key];
    }

    bool containsSubTreeRhs(const U &node, const U &subTree) {
        if (node.matches(subTree)) {
            return true;  // Exact match found
        }

        for (const auto &child : node.getChildren()) {
            if (containsSubTreeRhs(*child, subTree)) {
                return true;  // Partial match found in the child subtree
            }
        }

        return false;  // No match found in this node and its children
    }

    std::vector<int> getRhsIndexes(const U &key, const bool &isFullMatch) {
        int index = 0;
        std::vector<int> result;
        for (const U &node : rhsVector) {
            if (isFullMatch && key.matches(node)) {
                result.push_back(index);

            } else if (!isFullMatch && containsSubTreeRhs(node, key)) {
                result.push_back(index);
            }
            index += 1;
        }
        return result;
    }

    int getStmtFromIndexMap(int index) {
        return indexStmtMap[index];
    }

    void insertPattern(T left, U right, int stmtNum) {
        int index = static_cast<int>(rhsVector.size());

        lhsMap[left].push_back(stmtNum);
        rhsVector.push_back(right);
        indexStmtMap[index] = stmtNum;
    }

    std::vector<int> getAllMatch(const T &left, const U &right, bool isFullMatch) {
        std::vector<int> result;
        // get index of all matches
        std::vector<int> lhsStmt = getLhsValues(left);
        // get all stmt lhs
        std::vector<int> rhsIndexes;
        if (isFullMatch) {
            rhsIndexes = getRhsIndexes(right, true);
        } else {
            rhsIndexes = getRhsIndexes(right, false);
        }
        std::vector<int> rhsStmt;
        for (int index : rhsIndexes) {
            auto it = getStmtFromIndexMap(index);
            rhsStmt.push_back(it);
        }

        // match
        std::sort(lhsStmt.begin(), lhsStmt.end());
        std::sort(rhsStmt.begin(), rhsStmt.end());

        // Find common elements by comparing sorted lists
        std::set_intersection(lhsStmt.begin(), lhsStmt.end(),
                              rhsStmt.begin(), rhsStmt.end(),
                              std::back_inserter(result));
        return result;
    }

    std::vector<int> getRhsValues(U &rhsKey, bool isFullMatch) {
        std::vector<int> result;
        // get all stmt lhs
        std::vector<int> rhsIndexes;
        if (isFullMatch) {
            rhsIndexes = getRhsIndexes(rhsKey, true);
        } else {
            rhsIndexes = getRhsIndexes(rhsKey, false);
        }
        for (int index : rhsIndexes) {
            auto it = getStmtFromIndexMap(index);
            result.push_back(it);
        }
        return result;
    }
};