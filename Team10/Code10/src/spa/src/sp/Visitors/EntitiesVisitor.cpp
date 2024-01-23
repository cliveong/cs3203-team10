#include "EntitiesVisitor.h"

EntitiesVisitor::EntitiesVisitor(WriteFacade& writeFacade,
                                 std::shared_ptr<ASTNode> rootNode) : Visitor(writeFacade, rootNode) {}

void EntitiesVisitor::visitProgramNode(std::shared_ptr<ASTNode> node,
                                       std::vector<ASTNode>& varNodes) {
    for (const auto& child : node->getChildren()) {
        procedureNames.insert(child->getValue());
    }
    for (const auto& child : node->getChildren()) {
        child->accept(*this, varNodes);
    }
}

void EntitiesVisitor::visitProcedureNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    std::string send = node->getValue();
    writeFacade.storeProcedureName(send);
    varNodes.push_back(*node);

    for (const auto& child : node->getChildren()) {
        child->accept(*this, varNodes);
    }
}

void EntitiesVisitor::visitVarNode(std::shared_ptr<ASTNode> node,
                                   std::vector<ASTNode>& varNodes) {
    std::string send = node->getValue();
    writeFacade.storeVariable(send);
}

void EntitiesVisitor::visitAssignNode(std::shared_ptr<ASTNode> node,
                                      std::vector<ASTNode>& varNodes) {
    writeFacade.storeAssign(node->getStmtNum());
    writeFacade.storeStmt(node->getStmtNum());

    for (const auto& child : node->getChildren()) {
        child->accept(*this, varNodes);
    }
}

void EntitiesVisitor::visitContainerNode(
    std::shared_ptr<ASTNode> node,
    std::vector<ASTNode>& varNodes) {
    if (node->getType() == Constants::IF_STMT) {
        writeFacade.storeIf(node->getStmtNum());
    } else {
        writeFacade.storeWhile(node->getStmtNum());
    }

    writeFacade.storeStmt(node->getStmtNum());

    for (const auto& child : node->getChildren()) {
        child->accept(*this, varNodes);
    }
}

void EntitiesVisitor::visitExprNode(std::shared_ptr<ASTNode> node,
                                    std::vector<ASTNode>& varNodes) {
    if (node->getType() == Constants::VAR) {
        std::string send = node->getValue();
        writeFacade.storeVariable(send);
    } else if (node->getType() == Constants::CONST) {
        writeFacade.storeConstant(node->getValue());
    } else {
        for (const auto& child : node->getChildren()) {
            child->accept(*this, varNodes);
        }
    }
}

void EntitiesVisitor::visitPrintNode(std::shared_ptr<ASTNode> node,
                                     std::vector<ASTNode>& varNodes) {
    const auto& child = node->getChildren();
    writeFacade.storeStmt(node->getStmtNum());
    if (!child.empty()) {
        std::string sendValue = child[0]->getValue();
        writeFacade.storePrintStmt(child[0]->getStmtNum(), sendValue);
    }
}

void EntitiesVisitor::visitReadNode(std::shared_ptr<ASTNode> node,
                                    std::vector<ASTNode>& varNodes) {
    const auto& child = node->getChildren();
    writeFacade.storeStmt(node->getStmtNum());
    if (!child.empty()) {
        std::string sendValue = child[0]->getValue();
        writeFacade.storeReadStmt(child[0]->getStmtNum(), sendValue);
    }
}

void EntitiesVisitor::visitCallsNode(std::shared_ptr<ASTNode> node,
                                     std::vector<ASTNode>& varNodes) {
    std::string procName = node->getValue();
    int stmtNum = node->getStmtNum();
    writeFacade.storeStmt(stmtNum);

    if (procedureNames.find(procName) != procedureNames.end()) {
        writeFacade.storeCallStmtToProcName(stmtNum, procName);
    } else {
        throw std::runtime_error("Error: procedure '" + procName + "' does not exist.");
    }
}
