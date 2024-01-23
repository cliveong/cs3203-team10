#include "ExprNode.h"

#include "sp/Visitors/Visitor.h"

ExprNode::ExprNode() : ASTNode() {}
ExprNode::ExprNode(const std::string& name, const std::string& value,
                   int stmtNum)
    : ASTNode(name, value, stmtNum) {}

void ExprNode::accept(Visitor& visitor,
                      std::vector<ASTNode>& varNodes) {
    visitor.visitExprNode(shared_from_this(), varNodes);
}