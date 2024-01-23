#include "ModifiesVisitor.h"

ModifiesVisitor::ModifiesVisitor(WriteFacade& writeFacade,
                                 std::shared_ptr<ASTNode> rootNode)
    : Visitor(writeFacade, rootNode) {}

void ModifiesVisitor::visitProgramNode(std::shared_ptr<ASTNode> node,
                                       std::vector<ASTNode>& varNodes) {
    for (const auto& child : node->getChildren()) {
        child->additionalNodes.clear();
        child->accept(*this, varNodes);
    }
}

void ModifiesVisitor::visitProcedureNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> procVarNodes;

    if (node->additionalNodes.size() == 0) {
        for (const auto& child : node->getChildren()) {
            child->accept(*this, procVarNodes);
        }

        writeFacade.storeModifiesProc(*node, procVarNodes);
        node->setAdditionalNodes(procVarNodes);
    } else {

        procVarNodes = node->additionalNodes;
    }

    std::move(procVarNodes.begin(), procVarNodes.end(), std::back_inserter(varNodes));
}

void ModifiesVisitor::visitCallsNode(std::shared_ptr<ASTNode> node,
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

    writeFacade.storeModifies(*node, callsVarNodes);
    std::move(callsVarNodes.begin(), callsVarNodes.end(), std::back_inserter(varNodes));
}

void ModifiesVisitor::visitVarNode(std::shared_ptr<ASTNode> node,
                                   std::vector<ASTNode>& varNodes) {
    varNodes.push_back(*node);
}

void ModifiesVisitor::visitAssignNode(std::shared_ptr<ASTNode> node,
                                      std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> assignVarNodes;
    const auto& children = node->getChildren();
    if (!children.empty()) {
        const auto& child = children[0];
        if (child) {
            child->accept(*this, assignVarNodes);
        }
    }

    for (const ASTNode& useNode : assignVarNodes) {
        varNodes.push_back(useNode);
        writeFacade.storeModifies(*node, {useNode});
    }
}

void ModifiesVisitor::visitContainerNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> containerVarNodes;

    const auto& children = node->getChildren();
    for (int i = 1; i < children.size(); ++i) {
        ASTNode child = *children[i];

        for (const auto& grandChild : child.getChildren()) {
            grandChild->accept(*this, containerVarNodes);
        }
    }

    for (const ASTNode& useNode : containerVarNodes) {
        writeFacade.storeModifies(*node, {useNode});
    }

    std::move(containerVarNodes.begin(), containerVarNodes.end(), std::back_inserter(varNodes));
}

void ModifiesVisitor::visitReadNode(std::shared_ptr<ASTNode> node,
                                    std::vector<ASTNode>& varNodes) {
    const auto& children = node->getChildren();
    if (!children.empty()) {
        const auto& child = children[0];
        if (child) {
            writeFacade.storeModifies(*node, {*child});
            varNodes.push_back(*child);
        }
    }
}