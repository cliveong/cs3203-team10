#include "CallsStarVisitor.h"

CallsStarVisitor::CallsStarVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode) : Visitor(writeFacade, rootNode) {
    extractCallsStar(rootNode);
}
std::stack<std::shared_ptr<ASTNode>> procedureNodeStack;
std::unordered_map<std::string, std::vector<std::string>> callsNodesMapT;
std::unordered_map<std::string, std::vector<std::string>> transitiveCallsMapV;

void CallsStarVisitor::visitCallsNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) {
    Visitor::visitCallsNode(node, varNodes);

    if (!procedureNodeStack.empty()) {
        const auto caller = procedureNodeStack.top();
        const auto callee = node;
        callsNodesMapT[caller->getValue()].push_back(callee->getValue());
    }
}

void CallsStarVisitor::extractCallsStar(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    visitASTNode(rootNode, varNodes);
    extractCallsTransitive(rootNode);
}

void CallsStarVisitor::visitProcedureNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) {
    procedureNodeStack.push(node);
    Visitor::visitProcedureNode(node, varNodes);
    procedureNodeStack.pop();
}

void CallsStarVisitor::extractCallsTransitive(std::shared_ptr<ASTNode> rootNode) {
    for (const auto& entry : callsNodesMapT) {
        const auto& caller = entry.first;
        for (const auto& callee : entry.second) {
            transitiveCallsMapV[caller].push_back(callee);
            populateTransitiveCalls(caller, callee);
        }
    }

    for (const auto& entry : transitiveCallsMapV) {
        const auto& parent = entry.first;
        const auto& callees = entry.second;
        writeFacade.storeCalls(parent, callees, true);
    }
}

void CallsStarVisitor::populateTransitiveCalls(const std::string& caller, const std::string& callee) {
    auto it = callsNodesMapT.find(callee);
    if (it != callsNodesMapT.end()) {
        const std::vector<std::string>& calleeCalls = it->second;
        for (const auto& indirectCallee : calleeCalls) {
            if (indirectCallee != caller) {
                transitiveCallsMapV[caller].push_back(indirectCallee);
                populateTransitiveCalls(caller, indirectCallee);
            } else {
                throw std::runtime_error("Circular dependency detected.");
            }
        }
    }
}