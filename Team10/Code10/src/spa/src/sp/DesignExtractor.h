#pragma once

#include <stdio.h>

#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../commons/ASTNode.h"
#include "../commons/ProcedureNode.h"
#include "pkb/WriteFacade.h"

using namespace std;

class DesignExtractor {
   private:
    WriteFacade& writeFacade;
    std::shared_ptr<ASTNode> rootNode;

   public:
    DesignExtractor(WriteFacade& wf, std::shared_ptr<ASTNode> rootNode);
    void extract();
    void extractEntities(std::shared_ptr<ASTNode> rootNode);
    void extractFollows(std::shared_ptr<ASTNode> rootNode);
    void extractParents(std::shared_ptr<ASTNode> rootNode);
    void extractPattern(std::shared_ptr<ASTNode> rootNode);
    void extractUses(std::shared_ptr<ASTNode> node);
    void extractModifies(std::shared_ptr<ASTNode> node);
    void extractCalls(std::shared_ptr<ASTNode> rootNode);
    void extractCallsStar(std::shared_ptr<ASTNode> rootNode);
    void extractCFG(std::shared_ptr<ASTNode> rootNode);
    void clearNodes();
};
