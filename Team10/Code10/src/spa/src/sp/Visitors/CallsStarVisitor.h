#pragma once

#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "Visitor.h"

class CallsStarVisitor : public Visitor {
   public:
    CallsStarVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode);

    std::stack<std::shared_ptr<ASTNode>> procedureNodeStack;
    std::unordered_map<std::string, std::vector<std::string>> callsNodesMapT;
    std::unordered_map<std::string, std::vector<std::string>> transitiveCallsMapV;

    void visitCallsNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) override;

    void extractCallsStar(std::shared_ptr<ASTNode> rootNode);

    void visitProcedureNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) override;

    void extractCallsTransitive(std::shared_ptr<ASTNode> rootNode);

    void populateTransitiveCalls(const std::string& caller, const std::string& callee);
};
