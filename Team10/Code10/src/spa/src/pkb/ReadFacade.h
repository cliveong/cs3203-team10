#pragma once

#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Storage.h"
#include "pkb/relationship_managers/RelationshipManager.h"

class ReadFacade {
   private:
    std::shared_ptr<Storage> storageManager;

   public:
    // Constructor that takes in managers
    ReadFacade(std::shared_ptr<Storage> storageManager);

    std::vector<int> getFollows(const int& key, const bool& isStar) const;

    std::vector<int> getFollowsS2(const int& key, const bool& isStar) const;

    bool hasFollows() const;
    bool hasFollowsStar() const;

    std::vector<int> getParents(const int& key, const bool& isStar) const;

    std::vector<int> getParentsS2(const int& key, const bool& isStar) const;

    bool hasParents(const bool& isStar) const;

    std::vector<std::string> getUses(const int& number) const;

    bool getUses(const int& number, const std::string& variable) const;

    std::vector<std::string> getModifies(const int& number) const;

    bool getModifies(const int& number, const std::string& variable) const;

    bool isStringInVector(const std::vector<std::string>& vec, const std::string& str) const;

    std::vector<int> getAssign() const;

    std::vector<std::string> getConstant() const;

    std::vector<int> getIf() const;

    std::vector<int> getPrintS() const;

    std::vector<std::string> getProcedure() const;

    std::vector<int> getReadS() const;

    std::vector<int> getStmt() const;

    std::vector<std::string> getVariable() const;

    std::vector<int> getWhile() const;

    std::vector<int> getAssignPatternLhs(std::string varOrString) const;

    std::vector<int> getIfPattern(std::string& var) const;

    std::vector<int> getIfPatternStmt() const;
    std::vector<int> getWhilePatternStmt() const;

    std::vector<int> getWhilePattern(std::string& var) const;

    std::vector<int> getAssignPatternRhs(ASTNode expr, bool isFullMatch) const;

    std::vector<int> getAssignPattern(std::string lhs, ASTNode rhs, bool isFullMatch) const;

    std::string getCallStmtToProcName(const int stmtNum) const;

    std::vector<int> getAllCallStmtToProcName() const;

    bool hasCallVarName() const;

    std::vector<std::string> getUsesProc(const std::string value);

    bool getUsesProc(const std::string& key, const std::string& value);

    std::vector<std::string> getModifiesProc(const std::string value);

    bool getModifiesProc(const std::string& key, const std::string& value);

    std::vector<std::string> getCalls(const std::string& value, const bool& isStar);

    bool hasCalls(const bool& isStar);

    std::vector<std::string> getCallsS2(const std::string& value, const bool& isStar);

    std::string getPrintVarName(const int stmtNum) const;

    std::string getReadVarName(const int stmtNum) const;

    std::vector<int> getNext(const int stmtNum, const bool& isStar) const;

    std::vector<int> getNextS2(const int stmtNum, const bool& isStar) const;

    bool hasNext() const;
    bool hasNextStar() const;

    std::vector<int> getAffects(const int& stmtNum) const;
    std::vector<int> getAffectsS2(const int& stmtNum) const;
    bool hasAffects() const;

    void clearCache() const;
};
