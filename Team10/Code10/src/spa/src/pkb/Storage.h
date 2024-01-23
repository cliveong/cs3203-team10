#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../commons/ASTNode.h"
#include "../commons/CFGNode.h"
#include "entity_managers/AssignManager.h"
#include "entity_managers/ConstantManager.h"
#include "entity_managers/IfManager.h"
#include "entity_managers/PrintSManager.h"
#include "entity_managers/ProcedureManager.h"
#include "entity_managers/ReadSManager.h"
#include "entity_managers/StmtManager.h"
#include "entity_managers/VariableManager.h"
#include "entity_managers/WhileManager.h"
#include "pkb/cfg/Affects.h"
#include "pkb/cfg/Next.h"
#include "pkb/pattern/AssignPatternManager.h"
#include "pkb/relationship_managers/Calls.h"
#include "pkb/relationship_managers/CallsStar.h"
#include "pkb/relationship_managers/Follow.h"
#include "pkb/relationship_managers/IfPatternManager.h"
#include "pkb/relationship_managers/ModifiesProc.h"
#include "pkb/relationship_managers/ModifiesS.h"
#include "pkb/relationship_managers/Parent.h"
#include "pkb/relationship_managers/ParentStar.h"
#include "pkb/relationship_managers/PrintVarName.h"
#include "pkb/relationship_managers/ReadVarName.h"
#include "pkb/relationship_managers/StmtToProcName.h"
#include "pkb/relationship_managers/UsesProc.h"
#include "pkb/relationship_managers/UsesS.h"
#include "pkb/relationship_managers/WhilePatternManager.h"

class Storage {
   private:
    // Entity
    std::shared_ptr<AssignManager> assignManager;
    std::shared_ptr<ConstantManager> constantManager;
    std::shared_ptr<IfManager> ifManager;
    std::shared_ptr<PrintSManager> printSManager;
    std::shared_ptr<ProcedureManager> procedureManager;
    std::shared_ptr<ReadSManager> readSManager;
    std::shared_ptr<StmtManager> stmtManager;
    std::shared_ptr<VariableManager> variableManager;
    std::shared_ptr<WhileManager> whileManager;

    std::shared_ptr<Follow> followsManager;
    std::shared_ptr<Parent> parentManager;
    std::shared_ptr<ParentStar> parentStarManager;
    std::shared_ptr<UsesS> usesSManager;
    std::shared_ptr<ModifiesS> modifiesSManager;

    std::shared_ptr<AssignPatternManager> assignPatternManager;
    std::shared_ptr<IfPatternManager> ifPatternManager;
    std::shared_ptr<WhilePatternManager> whilePatternManager;
    std::shared_ptr<StmtToProcName> stmtToProcNameManager;
    std::shared_ptr<UsesProc> usesProcManager;
    std::shared_ptr<ModifiesProc> modifiesProcManager;
    std::shared_ptr<Calls> callsManager;
    std::shared_ptr<CallsStar> callsStarManager;
    std::shared_ptr<ReadVarName> readVarNameManager;
    std::shared_ptr<PrintVarName> printVarNameManager;
    std::shared_ptr<Next> nextManager;
    std::shared_ptr<Affects> affectsManager;

   public:
    // Initialise all managers in storage
    Storage();

    std::vector<int> getRsFromManagerIntInt(const std::shared_ptr<RelationshipManager<int, std::vector<int>>>& manager, const int& number) const;

    std::vector<std::string> getRsFromManagerStringString(const std::shared_ptr<RelationshipManager<std::string, std::vector<std::string>>>& manager, const std::string& number) const;

    std::vector<std::string> extractStringFromChild(const std::vector<ASTNode>& nodes) const;

    std::string getStringFromIntStringMap(const std::shared_ptr<RelationshipManager<int, std::string>>& manager, const int& stmtNum) const;

    std::vector<int> extractIntFromChild(const std::vector<ASTNode>& nodes) const;

    std::vector<int> getFollows(const int& key) const;

    std::vector<int> getFollows(const int& key, const bool& isStar) const;

    std::vector<int> getFollowsS2(const int& key, const bool& isStar) const;

    bool hasFollows() const;
    bool hasFollowsStar() const;

    std::vector<int> getParents(const int& key) const;

    std::vector<int> getParents(const int& key, const bool& isStar) const;

    std::vector<int> getParentsS2(const int& key) const;

    std::vector<int> getParentsS2(const int& key, const bool& isStar) const;

    bool hasParents() const;
    bool hasParentsStar() const;

    std::vector<std::string> getUses(const int& number) const;

    std::vector<std::string> getRsFromManagerIntString(
        const std::shared_ptr<
            RelationshipManager<int, std::vector<std::string>>>& manager,
        const int& number) const;

    bool getUses(const int& number, const std::string& variable) const;

    std::vector<std::string> getModifies(const int& number) const;

    bool getModifies(const int& number, const std::string& variable) const;

    bool isStringInVector(const std::vector<std::string>& vec,
                          const std::string& str) const;

    std::vector<int> getAssign() const;

    std::vector<std::string> getConstant() const;

    std::vector<int> getIf() const;

    std::vector<int> getPrintS() const;

    std::vector<std::string> getProcedure() const;

    std::vector<int> getReadS() const;

    std::vector<int> getStmt() const;

    std::vector<std::string> getVariable() const;

    std::vector<int> getWhile() const;

    std::vector<int> getAssignPatternLhs(std::string& varOrString) const;

    std::vector<int> getIfPattern(std::string& var) const;
    std::vector<int> getIfPatternStmt() const;
    std::vector<int> getWhilePatternStmt() const;

  std::vector<int> getWhilePattern(std::string& var) const;

    std::vector<int> getAssignPatternRhs(ASTNode& expr) const;

    std::vector<int> getAssignPatternRhsFull(ASTNode& expr) const;

    std::vector<int> getAssignPattern(std::string& lhs, ASTNode& rhs) const;

    std::vector<int> getAssignPatternFull(std::string& lhs, ASTNode& rhs) const;

    void storeAssign(int stmtNum);
    void storeConstant(std::string value);
    void storeIf(int stmtNum);
    void storePrintStmt(int stmtNum, std::string& varName);
    void storeProcedureName(std::string& procName);
    void storeReadStmt(int stmtNum, std::string& varName);
    void storeStmt(int stmtNum);
    void storeVariable(std::string& varName);
    void storeWhile(int stmtNum);

    void storeFollows(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeParents(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeParentStar(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);

    void storeUsesS(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    void storeModifiesS(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);

    void storeAssignPattern(const std::string& lhs, const ASTNode& rhs, int stmtNum);
    void storeIfPattern(const std::vector<std::string>& variables, int stmtNum);
    void storeWhilePattern(const std::vector<std::string>& variables, int stmtNum);

    void storeStmtToProcName(const int stmtNum, std::string& procName);
    std::string getStmtToProcName(const int stmtNum) const;
    std::vector<int> getAllCallStmtToProcName() const;
    bool hasCallVarName() const;

    void storeUsesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    std::vector<std::string> getUsesProc(const std::string value);
    bool getUsesProc(const std::string& key, const std::string& value);

    void storeModifiesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes);
    std::vector<std::string> getModifiesProc(const std::string value) const;
    bool getModifiesProc(const std::string& key, const std::string& value) const;

    void storeCalls(const std::string& headNode, const std::vector<std::string>& childrenNodes);
    std::vector<std::string> getCalls(const std::string& value);
    bool hasCalls();
    std::vector<std::string> getCallsS2(const std::string& value);

    void storeCallsStar(const std::string& headNode, const std::vector<std::string>& childrenNodes);
    std::vector<std::string> getCallsStar(const std::string& value);
    bool hasCallsStar();
    std::vector<std::string> getCallsStarS2(const std::string& value);

    void storePrintVarName(const int stmtNum, std::string& procName);
    std::string getPrintVarName(const int stmtNum) const;

    void storeReadVarName(const int stmtNum, std::string& procName);
    std::string getReadVarName(const int stmtNum) const;

    void storeNext(CFGNode& headNode);
    std::vector<int> getNext(const int& stmtNum) const;
    std::vector<int> getNextStar(const int& stmtNum) const;
    std::vector<int> getNextS2(const int& stmtNum) const;
    std::vector<int> getNextStarS2(const int& stmtNum) const;
    bool hasNext() const;
    bool hasNextStar();

    std::vector<int> getAffects(const int& stmtNum) const;
    std::vector<int> getAffectsS2(const int& stmtNum) const;
    bool hasAffects() const;

    void dfs(const int& stmt, const std::string& v, std::vector<int>& result, std::unordered_set<int>& visited) const;
    void dfsReverse(const int& stmt, const std::string& v, std::unordered_set<int>& result,
                    std::unordered_set<int>& visited) const;

    void clearCacheNext() const;
    void clearCacheAffects();
    void clearIfPattern() const;
    void clearWhilePattern() const;
    void clearCallsS2() const;
    void clearParents() const;
    void clearFollows() const;
};