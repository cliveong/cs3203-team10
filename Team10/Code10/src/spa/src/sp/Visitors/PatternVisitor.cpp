#include "PatternVisitor.h"

PatternVisitor::PatternVisitor(WriteFacade& writeFacade,
                               std::shared_ptr<ASTNode> rootNode)
    : Visitor(writeFacade, rootNode) {}

void PatternVisitor::visitAssignNode(std::shared_ptr<ASTNode> node,
                                     std::vector<ASTNode>& varNodes) {
    auto children = node->getChildren();
    if (!children.empty()) {
        const auto& leftChild = children[0];
        if (node->getChildren().size() > 1) {
            const auto& rightChild = children[1];
            writeFacade.storeAssignPattern(*leftChild, *rightChild,
                                           node->getStmtNum());
        }
    }
}

void PatternVisitor::visitContainerNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    auto children = node->getChildren();
    std::vector<ASTNode> containerNodes;

    if (!children.empty()) {
        const auto& condExpr = children[0];
        condExpr->accept(*this, containerNodes);
    }

    for (int i = 1; i < children.size(); i++) {
        children[i]->accept(*this, varNodes);
    }

    std::vector<std::string> variables;

    for (const auto& vars : containerNodes) {
        variables.push_back(vars.getValue());
    }

    if (variables.size() > 0) {
        if (node->getType() == Constants::IF_STMT) {
            writeFacade.storeIfPattern(variables, node->getStmtNum());
        } else {
            writeFacade.storeWhilePattern(variables, node->getStmtNum());
        }
    }
}

void PatternVisitor::visitExprNode(std::shared_ptr<ASTNode> node,
                                   std::vector<ASTNode>& varNodes) {
    if (node->getType() == Constants::VAR) {
        varNodes.push_back(*node);
    } else {
        for (const auto& child : node->getChildren()) {
            child->accept(*this, varNodes);
        }
    }
}