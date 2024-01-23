#pragma once

#include <set>

#include "Visitor.h"

class EntitiesVisitor : public Visitor {
   public:
    EntitiesVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    std::set<std::string> procedureNames;

    void visitProgramNode(std::shared_ptr<ASTNode> node,
                          std::vector<ASTNode>& varNodes) override;

    void visitProcedureNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override;

    void visitVarNode(std::shared_ptr<ASTNode> node,
                      std::vector<ASTNode>& varNodes);

    void visitAssignNode(std::shared_ptr<ASTNode> node,
                         std::vector<ASTNode>& varNodes) override;

    void visitContainerNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override;

    void visitExprNode(std::shared_ptr<ASTNode> node,
                       std::vector<ASTNode>& varNodes) override;

    void visitPrintNode(std::shared_ptr<ASTNode> node,
                        std::vector<ASTNode>& varNodes) override;

    void visitReadNode(std::shared_ptr<ASTNode> node,
                       std::vector<ASTNode>& varNodes) override;

    void visitCallsNode(std::shared_ptr<ASTNode> node,
                        std::vector<ASTNode>& varNodes) override;
};