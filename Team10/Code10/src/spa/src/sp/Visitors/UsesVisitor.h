#pragma once

#include "Visitor.h"

class UsesVisitor : public Visitor {
   public:
    UsesVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    void visitProgramNode(std::shared_ptr<ASTNode> node,
                          std::vector<ASTNode>& varNodes) override;

    void visitProcedureNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override;

    void visitCallsNode(std::shared_ptr<ASTNode> node,
                        std::vector<ASTNode>& varNodes) override;

    void visitVarNode(std::shared_ptr<ASTNode> node,
                      std::vector<ASTNode>& varNodes) override;

    void visitAssignNode(std::shared_ptr<ASTNode> node,
                         std::vector<ASTNode>& varNodes) override;

    void visitContainerNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override;

    void visitExprNode(std::shared_ptr<ASTNode> node,
                       std::vector<ASTNode>& varNodes) override;

    void visitPrintNode(std::shared_ptr<ASTNode> node,
                        std::vector<ASTNode>& varNodes) override;
};