#pragma once

#include <string>

#include "ASTNode.h"

class ReadNode : public ASTNode {
   public:
    ReadNode(int stmtNum);
    void accept(Visitor& visitor, std::vector<ASTNode>& varNodes) override;
};