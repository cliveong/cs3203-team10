#include "ProcedureNode.h"

#include "sp/Visitors/Visitor.h"

ProcedureNode::ProcedureNode(const std::string& name)
    : ASTNode(Constants::PROCEDURE, name, -1) {}

void ProcedureNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitProcedureNode(shared_from_this(), varNodes);
};