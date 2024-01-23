#include "AssignNode.h"

#include "sp/Visitors/Visitor.h"

AssignNode::AssignNode(int stmtNum) : ASTNode(Constants::ASSIGN_STMT, "", stmtNum) {}

void AssignNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitAssignNode(shared_from_this(), varNodes);
}