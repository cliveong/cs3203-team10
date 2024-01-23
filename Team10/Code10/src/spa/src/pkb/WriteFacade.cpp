#include "WriteFacade.h"

#include <vector>

#include "Storage.h"

WriteFacade::WriteFacade(std::shared_ptr<Storage>& storageManager)
    : storageManager(storageManager) {}

void WriteFacade::storeAssign(int stmtNum) {
    storageManager->storeAssign(stmtNum);
}

void WriteFacade::storeConstant(std::string value) {
    storageManager->storeConstant(std::move(value));
}

void WriteFacade::storeIf(int stmtNum) {
    storageManager->storeIf(stmtNum);
}

void WriteFacade::storePrintStmt(int stmtNum, std::string& varName) {
    storageManager->storePrintStmt(stmtNum, varName);
}

void WriteFacade::storeProcedureName(std::string& procName) {
    storageManager->storeProcedureName(procName);
}

void WriteFacade::storeReadStmt(int stmtNum, std::string& varName) {
    storageManager->storeReadStmt(stmtNum, varName);
}

void WriteFacade::storeStmt(int stmtNum) {
    storageManager->storeStmt(stmtNum);
}

void WriteFacade::storeVariable(std::string& varName) {
    storageManager->storeVariable(varName);
}

void WriteFacade::storeWhile(int stmtNum) {
    storageManager->storeWhile(stmtNum);
}

void WriteFacade::storeFollows(const ASTNode&
                                   firstNode,
                               const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeFollows(firstNode, childrenNodes);
}

void WriteFacade::storeParents(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeParents(firstNode, childrenNodes);
}

void WriteFacade::storeParentStar(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeParentStar(firstNode, childrenNodes);
}

void WriteFacade::storeParents(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes, const bool& isStar) {
    if (isStar) {
        storageManager->storeParentStar(firstNode, childrenNodes);
    } else {
        storageManager->storeParents(firstNode, childrenNodes);
    }
}

void WriteFacade::storeUses(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeUsesS(firstNode, childrenNodes);
}

void WriteFacade::storeModifies(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeModifiesS(firstNode, childrenNodes);
}

void WriteFacade::storeAssignPattern(const ASTNode& lhs, const ASTNode& rhs, int stmtNum) {
    std::string lhsString = lhs.getValue();
    storageManager->storeAssignPattern(lhsString, rhs, stmtNum);
}
void WriteFacade::storeIfPattern(const std::vector<std::string>& variables, int stmtNum) {
    storageManager->storeIfPattern(variables, stmtNum);
}

void WriteFacade::storeWhilePattern(const std::vector<std::string>& variables, int stmtNum) {
    storageManager->storeWhilePattern(variables, stmtNum);
}

void WriteFacade::storeCallStmtToProcName(const int stmtNum, std::string procName) {
    storageManager->storeStmtToProcName(stmtNum, procName);
}

void WriteFacade::storeUsesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeUsesProc(headNode, childrenNodes);
}

void WriteFacade::storeModifiesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes) {
    storageManager->storeModifiesProc(headNode, childrenNodes);
}

void WriteFacade::storeCalls(const std::string& headNode, const std::vector<std::string>& childrenNodes, const bool& isStar) {
    if (isStar) {
        storageManager->storeCallsStar(headNode, childrenNodes);
    } else {
        storageManager->storeCalls(headNode, childrenNodes);
    }
}

void WriteFacade::storeNext(CFGNode& headNode) {
    storageManager->storeNext(headNode);
}
