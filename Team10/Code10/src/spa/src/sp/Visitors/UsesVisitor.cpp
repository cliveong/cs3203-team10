#include "UsesVisitor.h"

UsesVisitor::UsesVisitor(WriteFacade& writeFacade,
                         std::shared_ptr<ASTNode> rootNode)
    : Visitor(writeFacade, rootNode) {}

void UsesVisitor::visitProgramNode(std::shared_ptr<ASTNode> node,
                                   std::vector<ASTNode>& varNodes) {
    for (const auto& child : node->getChildren()) {
        child->additionalNodes.clear();
        child->accept(*this, varNodes);
    }
}

void UsesVisitor::visitProcedureNode(std::shared_ptr<ASTNode> node,
                                     std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> procVarNodes;

    if (node->additionalNodes.size() == 0) {
        for (const auto& child : node->getChildren()) {
            child->accept(*this, procVarNodes);
        }

        writeFacade.storeUsesProc(*node, procVarNodes);
        node->setAdditionalNodes(procVarNodes);
    } else {
        procVarNodes = node->additionalNodes;
    }

    std::move(procVarNodes.begin(), procVarNodes.end(), std::back_inserter(varNodes));
}

void UsesVisitor::visitCallsNode(std::shared_ptr<ASTNode> node,
                                 std::vector<ASTNode>& varNodes) {
    std::shared_ptr<ASTNode> procNode;
    std::vector<ASTNode> callsVarNodes;
    for (const auto& child : rootNode->getChildren()) {
        if (child->getValue() == node->getValue()) {
            procNode = child;
            break;
        }
    }
    procNode->accept(*this, callsVarNodes);

    writeFacade.storeUses(*node, callsVarNodes);

    std::move(callsVarNodes.begin(), callsVarNodes.end(), std::back_inserter(varNodes));
}

void UsesVisitor::visitVarNode(std::shared_ptr<ASTNode> node,
                               std::vector<ASTNode>& varNodes) {
    varNodes.push_back(*node);
}

void UsesVisitor::visitAssignNode(std::shared_ptr<ASTNode> node,
                                  std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> assignVarNodes;
    const auto& children = node->getChildren();
    if (!children.empty()) {
        const auto& child = children[1];
        if (child) {
            child->accept(*this, assignVarNodes);
        }
    }

    for (const ASTNode& useNode : assignVarNodes) {
        varNodes.push_back(useNode);
        writeFacade.storeUses(*node, {useNode});
    }
}

void UsesVisitor::visitContainerNode(std::shared_ptr<ASTNode> node,
                                     std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> containerVarNodes;

    const auto& children = node->getChildren();
    for (int i = 0; i < children.size(); ++i) {
        ASTNode child = *children[i];

        for (const auto& grandChild : child.getChildren()) {
            grandChild->accept(*this, containerVarNodes);
        }
    }

    for (const ASTNode& useNode : containerVarNodes) {
        writeFacade.storeUses(*node, {useNode});
    }

    std::move(containerVarNodes.begin(), containerVarNodes.end(), std::back_inserter(varNodes));
}

void UsesVisitor::visitExprNode(std::shared_ptr<ASTNode> node,
                                std::vector<ASTNode>& varNodes) {
    if (node->getType() == Constants::VAR) {
        varNodes.push_back(*node);
    } else {
        for (const auto& child : node->getChildren()) {
            child->accept(*this, varNodes);
        }
    }
}

void UsesVisitor::visitPrintNode(std::shared_ptr<ASTNode> node,
                                 std::vector<ASTNode>& varNodes) {
    const auto& children = node->getChildren();
    if (!children.empty()) {
        const auto& child = children[0];
        if (child) {
            writeFacade.storeUses(*node, {*child});
            varNodes.push_back(*child);
        }
    }
}