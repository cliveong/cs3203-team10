#include "PrintNode.h"

#include "sp/Visitors/Visitor.h"

PrintNode::PrintNode(int stmtNum)
    : ASTNode(Constants::PRINT, "", stmtNum) {}

void PrintNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitPrintNode(shared_from_this(), varNodes);
}