#pragma once

#include "Visitor.h"

class ParentsVisitor : public Visitor {
   public:
    ParentsVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    void visitContainerNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override;
};