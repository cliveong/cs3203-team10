#pragma once

#include <string>

#include "ASTNode.h"

class VarNode : public ASTNode {
   public:
    VarNode(const std::string& name, int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};