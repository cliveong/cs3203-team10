#include "CallNode.h"

#include "sp/Visitors/Visitor.h"

CallNode::CallNode(const std::string& procName, int stmtNum)
    : ASTNode(Constants::CALL, procName, stmtNum) {}

void CallNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitCallsNode(shared_from_this(), varNodes);
}