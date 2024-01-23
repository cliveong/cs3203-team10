#include "ProgramNode.h"

#include "sp/Visitors/Visitor.h"

ProgramNode::ProgramNode()
    : ASTNode(Constants::PROGRAM, "", -1) {}

void ProgramNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitProgramNode(shared_from_this(), varNodes);
};