#include "VarNode.h"

#include "sp/Visitors/Visitor.h"

VarNode::VarNode(const std::string& name, int stmtNum) : ASTNode(Constants::VAR, name, stmtNum) {}

void VarNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitVarNode(shared_from_this(), varNodes);
}