#include "ParentsVisitor.h"

ParentsVisitor::ParentsVisitor(WriteFacade& writeFacade,
                               std::shared_ptr<ASTNode> rootNode)
    : Visitor(writeFacade, rootNode) {}

void ParentsVisitor::visitContainerNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    std::vector<ASTNode> indirectNodes;
    std::vector<ASTNode> directNodes;
    const auto& children = node->getChildren();

    for (int i = 1; i < children.size(); i++) {
        for (const auto& grandChild : children[i]->getChildren()) {
            directNodes.push_back(*grandChild);
            indirectNodes.push_back(*grandChild);
            grandChild->accept(*this, indirectNodes);
        }
    }
    writeFacade.storeParents(*node, directNodes);
    writeFacade.storeParentStar(*node, indirectNodes);

    varNodes.insert(varNodes.end(), indirectNodes.begin(), indirectNodes.end());
}