#pragma once

#include "Visitor.h"

class PatternVisitor : public Visitor {
   public:
    PatternVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    void visitAssignNode(std::shared_ptr<ASTNode> node,
                         std::vector<ASTNode>& varNodes) override;

    void visitContainerNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override;

    void visitExprNode(std::shared_ptr<ASTNode> node,
                       std::vector<ASTNode>& varNodes) override;
};