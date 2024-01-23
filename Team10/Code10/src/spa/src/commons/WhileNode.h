#pragma once

#include <string>

#include "ASTNode.h"

class WhileNode : public ASTNode {
   public:
    WhileNode(int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};