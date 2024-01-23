#include "ConstNode.h"

ConstNode::ConstNode(const std::string& name, int stmtNum)
    : ASTNode(Constants::CONST, name, stmtNum) {}