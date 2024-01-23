#pragma once

#include <memory>
#include <string>

#include "../commons/ASTNode.h"
#include "../commons/CFGNode.h"
#include "Storage.h"
#include "interfaces/IWriteRelationship.h"
#include "pattern/IWritePattern.h"
#include "pkb/entity_managers/EntityManager.h"
#include "pkb/interfaces/IWriteEntity.h"

class WriteFacade {
   private:
    std::shared_ptr<Storage>& storageManager;

   public:
    explicit WriteFacade(std::shared_ptr<Storage>& storageManager);

    // Entities
    void storeAssign(int stmtNum);
    void storeConstant(std::string value);
    void storeIf(int stmtNum);
    void storePrintStmt(int stmtNum, std::string& varName);
    void storeProcedureName(std::string& procName);
    void storeReadStmt(int stmtNum, std::string& varName);
    void storeStmt(int stmtNum);
    void storeVariable(std::string& varName);
    void storeWhile(int stmtNum);

    // Relationships
    void storeFollows(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);

    void storeParents(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeParentStar(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeParents(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes, const bool& isStar);  // Merged

    void storeUses(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeModifies(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);

    void storeUsesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeModifiesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);

    void storeAssignPattern(const ASTNode& lhs, const ASTNode& rhs, int stmtNum);
    void storeIfPattern(const std::vector<std::string>& variables, int stmtNum);
    void storeWhilePattern(const std::vector<std::string>& variables, int stmtNum);

    void storeCalls(const std::string& headNode, const std::vector<std::string>& childrenNodes, const bool& isStar);

    void storeCallStmtToProcName(const int stmtNum, std::string procName);

    void storeNext(CFGNode& headNode);
};
