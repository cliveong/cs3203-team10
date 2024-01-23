#pragma once

#include "Visitor.h"

class ModifiesVisitor : public Visitor {
   public:
    ModifiesVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

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

    void visitReadNode(std::shared_ptr<ASTNode> node,
                       std::vector<ASTNode>& varNodes) override;
};