#include "StmtListNode.h"

StmtListNode::StmtListNode()
    : ASTNode(Constants::STMT_LIST, "", -1) {}

void StmtListNode::accept(Visitor& vistor, std::vector<ASTNode>& varNodes) {
    vistor.visitStmtListNode(shared_from_this(), varNodes);
};