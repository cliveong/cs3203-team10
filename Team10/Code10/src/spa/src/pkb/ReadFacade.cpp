#include "ReadFacade.h"

#include <vector>

#include "Storage.h"

ReadFacade::ReadFacade(std::shared_ptr<Storage> storageManager)
    : storageManager(storageManager) {}

std::vector<int> ReadFacade::getFollows(const int& key, const bool& isStar) const {
    return storageManager->getFollows(key, isStar);
}

std::vector<int> ReadFacade::getFollowsS2(const int& key, const bool& isStar) const {
    return storageManager->getFollowsS2(key, isStar);
}

bool ReadFacade::hasFollows() const {
    return storageManager->hasFollows();
}

bool ReadFacade::hasFollowsStar() const {
    return storageManager->hasFollowsStar();
}

std::vector<int> ReadFacade::getParents(const int& key, const bool& isStar) const {
    return storageManager->getParents(key, isStar);
}

std::vector<int> ReadFacade::getParentsS2(const int& key, const bool& isStar) const {
    return storageManager->getParentsS2(key, isStar);
}

bool ReadFacade::hasParents(const bool& isStar) const {
    if (isStar) {
        return storageManager->hasParentsStar();
    }
    return storageManager->hasParents();
}

std::vector<std::string> ReadFacade::getUses(const int& number) const {
    return storageManager->getUses(number);
}

bool ReadFacade::getUses(const int& number, const std::string& variable) const {
    return storageManager->getUses(number, variable);
}

std::vector<std::string> ReadFacade::getModifies(const int& number) const {
    return storageManager->getModifies(number);
}

bool ReadFacade::getModifies(const int& number, const std::string& variable) const {
    return storageManager->getModifies(number, variable);
}

std::vector<int> ReadFacade::getAssign() const {
    return storageManager->getAssign();
}

std::vector<std::string> ReadFacade::getConstant() const {
    return storageManager->getConstant();
}

std::vector<int> ReadFacade::getIf() const {
    return storageManager->getIf();
}

std::vector<int> ReadFacade::getPrintS() const {
    return storageManager->getPrintS();
}

std::vector<std::string> ReadFacade::getProcedure() const {
    return storageManager->getProcedure();
}

std::vector<int> ReadFacade::getReadS() const {
    return storageManager->getReadS();
}

std::vector<int> ReadFacade::getStmt() const {
    return storageManager->getStmt();
}

std::vector<std::string> ReadFacade::getVariable() const {
    return storageManager->getVariable();
}

std::vector<int> ReadFacade::getWhile() const {
    return storageManager->getWhile();
}

std::vector<int> ReadFacade::getAssignPatternLhs(std::string varOrString) const {
    return storageManager->getAssignPatternLhs(varOrString);
}

std::vector<int> ReadFacade::getWhilePattern(std::string& var) const {
    return storageManager->getWhilePattern(var);
}

std::vector<int> ReadFacade::getIfPattern(std::string& var) const {
    return storageManager->getIfPattern(var);
}

std::vector<int> ReadFacade::getIfPatternStmt() const {
    return storageManager->getIfPatternStmt();
}

std::vector<int> ReadFacade::getWhilePatternStmt() const {
    return storageManager->getWhilePatternStmt();
}

std::vector<int> ReadFacade::getAssignPatternRhs(ASTNode expr, bool isFullMatch) const {
    if (isFullMatch) {
        return storageManager->getAssignPatternRhsFull(expr);
    }
    return storageManager->getAssignPatternRhs(expr);
}

std::vector<int> ReadFacade::getAssignPattern(std::string lhs, ASTNode rhs, bool isFullMatch) const {
    if (isFullMatch) {
        return storageManager->getAssignPatternFull(lhs, rhs);
    }
    return storageManager->getAssignPattern(lhs, rhs);
}

std::string ReadFacade::getCallStmtToProcName(const int stmtNum) const {
    return storageManager->getStmtToProcName(stmtNum);
}

std::vector<int> ReadFacade::getAllCallStmtToProcName() const {
    return storageManager->getAllCallStmtToProcName();
}

bool ReadFacade::hasCallVarName() const {
    return storageManager->hasCallVarName();
}

std::vector<std::string> ReadFacade::getUsesProc(const std::string value) {
    return storageManager->getUsesProc(value);
}

bool ReadFacade::getUsesProc(const std::string& key, const std::string& value) {
    return storageManager->getUsesProc(key, value);
}

std::vector<std::string> ReadFacade::getModifiesProc(const std::string value) {
    return storageManager->getModifiesProc(value);
}

bool ReadFacade::getModifiesProc(const std::string& key, const std::string& value) {
    return storageManager->getModifiesProc(key, value);
}

std::vector<std::string> ReadFacade::getCalls(const std::string& value, const bool& isStar) {
    if (isStar) {
        return storageManager->getCallsStar(value);
    }
    return storageManager->getCalls(value);
}

std::vector<std::string> ReadFacade::getCallsS2(const std::string& value, const bool& isStar) {
    if (isStar) {
        return storageManager->getCallsStarS2(value);
    }
    return storageManager->getCallsS2(value);
}

bool ReadFacade::hasCalls(const bool& isStar) {
    if (isStar) {
        return storageManager->hasCallsStar();
    }
    return storageManager->hasCalls();
}

std::string ReadFacade::getPrintVarName(const int stmtNum) const {
    return storageManager->getPrintVarName(stmtNum);
}

std::string ReadFacade::getReadVarName(const int stmtNum) const {
    return storageManager->getReadVarName(stmtNum);
}

std::vector<int> ReadFacade::getAffects(const int& stmtNum) const {
    return storageManager->getAffects(stmtNum);
}

std::vector<int> ReadFacade::getAffectsS2(const int& stmtNum) const {
    return storageManager->getAffectsS2(stmtNum);
}

bool ReadFacade::hasAffects() const {
    return storageManager->hasAffects();
}

std::vector<int> ReadFacade::getNext(const int stmtNum, const bool& isStar) const {
    if (isStar) {
        return storageManager->getNextStar(stmtNum);
    }
    return storageManager->getNext(stmtNum);
}

std::vector<int> ReadFacade::getNextS2(const int stmtNum, const bool& isStar) const {
    if (isStar) {
        return storageManager->getNextStarS2(stmtNum);
    }
    return storageManager->getNextS2(stmtNum);
}

bool ReadFacade::hasNext() const {
    return storageManager->hasNext();
}

bool ReadFacade::hasNextStar() const {
    return storageManager->hasNextStar();
}

void ReadFacade::clearCache() const {
    storageManager->clearCacheAffects();
    storageManager->clearCacheNext();
    storageManager->clearCallsS2();
    storageManager->clearIfPattern();
    storageManager->clearWhilePattern();
    storageManager->clearParents();
    storageManager->clearFollows();
}