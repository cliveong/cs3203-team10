#pragma once

#include <string>

#include "ASTNode.h"

class ConstNode : public ASTNode {
   public:
    ConstNode(const std::string& name, int stmtNum);
};