#pragma once

#include <string>

#include "ASTNode.h"

class IfNode : public ASTNode {
   public:
    IfNode(int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};