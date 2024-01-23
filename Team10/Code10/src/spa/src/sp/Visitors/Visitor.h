#pragma once
#include "commons/Constants.h"
#include "pkb/WriteFacade.h"

class ASTNode;

class Visitor {
   public:
    Visitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode)
        : writeFacade(writeFacade), rootNode(rootNode){};

    virtual void visitVarNode(std::shared_ptr<ASTNode> node,
                              std::vector<ASTNode>& varNodes){};
    virtual void visitAssignNode(std::shared_ptr<ASTNode> node,
                                 std::vector<ASTNode>& varNodes){};
    virtual void visitProgramNode(std::shared_ptr<ASTNode> node,
                                  std::vector<ASTNode>& varNodes) {
        visitASTNode(node, varNodes);
    };
    virtual void visitProcedureNode(std::shared_ptr<ASTNode> node,
                                    std::vector<ASTNode>& varNodes) {
        visitASTNode(node, varNodes);
    };
    virtual void visitStmtListNode(std::shared_ptr<ASTNode> node,
                                   std::vector<ASTNode>& varNodes) {
        visitASTNode(node, varNodes);
    };
    virtual void visitContainerNode(std::shared_ptr<ASTNode> node,
                                    std::vector<ASTNode>& varNodes) {
        visitASTNode(node, varNodes);
    };
    virtual void visitPrintNode(std::shared_ptr<ASTNode> node,
                                std::vector<ASTNode>& varNodes){};
    virtual void visitReadNode(std::shared_ptr<ASTNode> node,
                               std::vector<ASTNode>& varNodes){};
    virtual void visitExprNode(std::shared_ptr<ASTNode> node,
                               std::vector<ASTNode>& varNodes){};
    virtual void visitCallsNode(std::shared_ptr<ASTNode> node,
                                std::vector<ASTNode>& varNodes){};
    virtual void visitASTNode(std::shared_ptr<ASTNode> node,
                              std::vector<ASTNode>& varNodes) {
        for (const auto& child : node->getChildren()) {
            child->accept(*this, varNodes);
        }
    };

    WriteFacade& writeFacade;
    std::shared_ptr<ASTNode> rootNode;
};
