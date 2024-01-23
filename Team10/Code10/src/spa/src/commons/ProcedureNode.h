#pragma once

#include <string>

#include "ASTNode.h"

class ProcedureNode : public ASTNode {
   public:
    ProcedureNode(const std::string& name);
    void accept(Visitor& vistor, std::vector<ASTNode>& varNodes) override;
};