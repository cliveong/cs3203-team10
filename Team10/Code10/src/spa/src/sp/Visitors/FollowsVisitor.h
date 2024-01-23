#pragma once

#include "Visitor.h"

class FollowsVisitor : public Visitor {
   public:
    FollowsVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    void visitStmtListNode(std::shared_ptr<ASTNode> node,
                           std::vector<ASTNode>& varNodes) override;
};