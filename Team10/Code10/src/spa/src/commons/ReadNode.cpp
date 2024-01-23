#include "ReadNode.h"

#include "sp/Visitors/Visitor.h"

ReadNode::ReadNode(int stmtNum)
    : ASTNode(Constants::READ, "", stmtNum) {}

void ReadNode::accept(Visitor& visitor,
                      std::vector<ASTNode>& varNodes) {
    visitor.visitReadNode(shared_from_this(), varNodes);
}