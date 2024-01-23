#pragma once

#include <string>

#include "ASTNode.h"

class CallNode : public ASTNode {
   public:
    CallNode(const std::string& name, int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};