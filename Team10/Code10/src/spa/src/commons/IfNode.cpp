#include "IfNode.h"

#include "sp/Visitors/Visitor.h"

IfNode::IfNode(int stmtNum) : ASTNode(Constants::IF_STMT, "", stmtNum) {}

void IfNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitContainerNode(shared_from_this(), varNodes);
}