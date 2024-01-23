#pragma once

#include <string>

#include "ASTNode.h"

class AssignNode : public ASTNode {
   public:
    AssignNode(int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};