#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "Visitor.h"

class CallsVisitor : public Visitor {
   public:
    CallsVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    std::stack<std::shared_ptr<ASTNode>> procedureNodeStack;
    std::unordered_map<std::string, std::vector<std::string>> callsNodesMapT;

    void visitCallsNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) override;

    void visitProcedureNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) override;

    void processCalls(std::shared_ptr<ASTNode> rootNode);
};
