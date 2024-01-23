#pragma once

#include <string>

#include "ASTNode.h"
#include "sp/Visitors/Visitor.h"

class StmtListNode : public ASTNode {
   public:
    StmtListNode();
    void accept(Visitor& vistor, std::vector<ASTNode>& varNodes) override;
};