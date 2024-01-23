#pragma once

#include <string>

#include "ASTNode.h"

class PrintNode : public ASTNode {
   public:
    PrintNode(int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};