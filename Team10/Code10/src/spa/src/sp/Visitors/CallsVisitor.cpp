#include "CallsVisitor.h"

CallsVisitor::CallsVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode) : Visitor(writeFacade, rootNode) {
    processCalls(rootNode);
}

void CallsVisitor::visitCallsNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) {
    Visitor::visitCallsNode(node, varNodes);

    if (!procedureNodeStack.empty()) {
        const auto caller = procedureNodeStack.top();
        const auto callee = node;
        callsNodesMapT[caller->getValue()].push_back(callee->getValue());
    }
}

void CallsVisitor::visitProcedureNode(std::shared_ptr<ASTNode> node, std::vector<ASTNode>& varNodes) {
    procedureNodeStack.push(node);
    Visitor::visitProcedureNode(node, varNodes);
    procedureNodeStack.pop();
}

void CallsVisitor::processCalls(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    visitASTNode(rootNode, varNodes);

    for (const auto& entry : callsNodesMapT) {
        const auto& parent = entry.first;
        const auto& callees = entry.second;
        writeFacade.storeCalls(parent, callees, false);
    }
}
