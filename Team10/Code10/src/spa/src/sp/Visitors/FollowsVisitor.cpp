#include "FollowsVisitor.h"

FollowsVisitor::FollowsVisitor(WriteFacade& writeFacade,
                               std::shared_ptr<ASTNode> rootNode)
    : Visitor(writeFacade, rootNode) {}

void FollowsVisitor::visitStmtListNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    const auto& children = node->getChildren();
    std::vector<std::pair<ASTNode, std::vector<ASTNode>>> nodePairs;
    std::vector<ASTNode> nodesAfter;

    for (int i = children.size() - 1; i >= 0; i--) {
        const std::shared_ptr<ASTNode> currentNode = children[i];
        if (i != children.size() - 1) {
            writeFacade.storeFollows(*currentNode, nodesAfter);
        }
        nodesAfter.insert(nodesAfter.begin(), *currentNode);
    }

    for (const auto& child : children) {
        child->accept(*this, varNodes);
    }
}