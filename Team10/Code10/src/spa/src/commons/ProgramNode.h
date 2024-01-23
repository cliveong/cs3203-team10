#pragma once

#include <string>

#include "ASTNode.h"

class ProgramNode : public ASTNode {
   public:
    ProgramNode();
    void accept(Visitor& vistor, std::vector<ASTNode>& varNodes) override;
};