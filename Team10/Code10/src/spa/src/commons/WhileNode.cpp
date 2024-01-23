#include "WhileNode.h"

#include "sp/Visitors/Visitor.h"

WhileNode::WhileNode(int stmtNum) : ASTNode(Constants::WHILE_STMT, "", stmtNum) {}

void WhileNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitContainerNode(shared_from_this(), varNodes);
}