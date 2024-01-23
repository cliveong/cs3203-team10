#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "Constants.h"

class CFGNode {
   public:
    CFGNode() : stmtNum(-1){};
    CFGNode(int stmtNum) : stmtNum(stmtNum){};

    void addSuccessor(std::shared_ptr<CFGNode> child) {
        successors.push_back(child);
    }

    void updateSuccessor(std::shared_ptr<CFGNode> newChild) {
        auto it = std::find_if(successors.begin(), successors.end(),
                               [](const std::shared_ptr<CFGNode>& successor) {
                                   return successor->getStmtNum() == -1;
                               });

        if (it != successors.end()) {
            *it = newChild;
        }
    }

    void addParents(std::shared_ptr<CFGNode> child) {
        parents.push_back(child);
    }

    int getStmtNum() {
        return stmtNum;
    }

    bool hasParents() {
        return parents.size() > 0;
    }

    std::vector<std::shared_ptr<CFGNode>> getSuccessors() {
        return successors;
    }

    std::vector<std::shared_ptr<CFGNode>> getParents() { return parents; }

   private:
    int stmtNum;
    std::vector<std::shared_ptr<CFGNode>> successors;
    std::vector<std::shared_ptr<CFGNode>> parents;
};
