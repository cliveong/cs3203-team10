#pragma once

#include <string>

#include "ASTNode.h"

class ExprNode : public ASTNode {
   public:
    ExprNode();
    ExprNode(const std::string& name, const std::string& value, int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};