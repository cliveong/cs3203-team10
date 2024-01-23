#include "Storage.h"

#include <utility>
#include <vector>

Storage::Storage() {
    assignManager = std::make_shared<AssignManager>();
    constantManager = std::make_shared<ConstantManager>();
    ifManager = std::make_shared<IfManager>();
    printSManager = std::make_shared<PrintSManager>();
    procedureManager = std::make_shared<ProcedureManager>();
    readSManager = std::make_shared<ReadSManager>();
    stmtManager = std::make_shared<StmtManager>();
    variableManager = std::make_shared<VariableManager>();
    whileManager = std::make_shared<WhileManager>();

    followsManager = std::make_shared<Follow>();
    parentManager = std::make_shared<Parent>();
    parentStarManager = std::make_shared<ParentStar>();
    usesSManager = std::make_shared<UsesS>();
    modifiesSManager = std::make_shared<ModifiesS>();

    assignPatternManager = std::make_shared<AssignPatternManager>();
    ifPatternManager = std::make_shared<IfPatternManager>();
    whilePatternManager = std::make_shared<WhilePatternManager>();
    stmtToProcNameManager = std::make_shared<StmtToProcName>();
    usesProcManager = std::make_shared<UsesProc>();
    modifiesProcManager = std::make_shared<ModifiesProc>();
    callsManager = std::make_shared<Calls>();
    callsStarManager = std::make_shared<CallsStar>();
    readVarNameManager = std::make_shared<ReadVarName>();
    printVarNameManager = std::make_shared<PrintVarName>();
    nextManager = std::make_shared<Next>();
    affectsManager = std::make_shared<Affects>();
}

std::vector<std::string> Storage::getRsFromManagerIntString(const std::shared_ptr<RelationshipManager<int, std::vector<std::string>>>& manager, const int& number) const {
    try {
        return manager->getRelationship(number);
    } catch (const std::out_of_range& e) {
        return std::vector<std::string>();
    }
}

std::vector<int> Storage::getRsFromManagerIntInt(const std::shared_ptr<RelationshipManager<int, std::vector<int>>>& manager, const int& number) const {
    try {
        return manager->getRelationship(number);
    } catch (const std::out_of_range& e) {
        return std::vector<int>();
    }
}

std::vector<std::string> Storage::getRsFromManagerStringString(const std::shared_ptr<RelationshipManager<std::string, std::vector<std::string>>>& manager, const std::string& number) const {
    try {
        return manager->getRelationship(number);
    } catch (const std::out_of_range& e) {
        return std::vector<std::string>();
    }
}

std::string Storage::getStringFromIntStringMap(const std::shared_ptr<RelationshipManager<int, std::string>>& manager, const int& stmtNum) const {
    try {
        return manager->getRelationship(stmtNum);
    } catch (const std::out_of_range& e) {
        return std::string();
    }
}

std::vector<int> Storage::getFollows(const int& key) const {
    return getRsFromManagerIntInt(followsManager, key);
}

std::vector<int> Storage::getFollows(const int& key, const bool& isStar) const {
    std::vector<int> temp = getFollows(key);

    if (!isStar && temp.size() > 0) {
        std::vector<int> follows(1);
        follows[0] = temp[0];
        return follows;
    } else {
        return temp;
    }
}

std::vector<int> Storage::getFollowsS2(const int& key, const bool& isStar) const {
    return followsManager->findKeysWithInt(key, isStar);
}

bool Storage::hasFollows() const {
    return followsManager->hasKeyValues();
}

bool Storage::hasFollowsStar() const {
    return followsManager->hasFollowStar();
}

std::vector<int> Storage::getParents(const int& key) const {
    return getRsFromManagerIntInt(parentManager, key);
}

std::vector<int> Storage::getParents(const int& key,
                                     const bool& isStar) const {
    if (isStar) {
        return getRsFromManagerIntInt(parentStarManager, key);
    } else {
        return getParents(key);
    }
}

std::vector<int> Storage::getParentsS2(const int& key) const {
    return parentManager->findKeysWithInt(key);
}

std::vector<int> Storage::getParentsS2(const int& key,
                                       const bool& isStar) const {
    if (isStar) {
        return parentStarManager->findKeysWithInt(key);
    } else {
        return parentManager->findKeysWithInt(key);
    }
}

bool Storage::hasParents() const {
    return parentManager->hasKeyValues();
}

bool Storage::hasParentsStar() const {
    return parentStarManager->hasKeyValues();
}

std::vector<std::string> Storage::getUses(const int& number) const {
    std::vector<std::string> result = {};
    result = getRsFromManagerIntString(usesSManager, number);
    return result;
}

bool Storage::getUses(const int& number, const std::string& variable) const {
    std::vector<std::string> usesVector = getUses(number);

    return isStringInVector(usesVector, variable);
}

std::vector<std::string> Storage::getModifies(const int& number) const {
    std::vector<std::string> result = {};
    result = getRsFromManagerIntString(modifiesSManager, number);
    return result;
}

bool Storage::getModifies(const int& number,
                          const std::string& variable) const {
    std::vector<std::string> modifiesVector = getModifies(number);

    return isStringInVector(modifiesVector, variable);
}

bool Storage::isStringInVector(const std::vector<std::string>& vec,
                               const std::string& str) const {
    // Use std::find to search for the string in the vector
    auto it = std::find(vec.begin(), vec.end(), str);

    // If std::find doesn't reach the end of the vector, the string is found
    return it != vec.end();
}

std::vector<int> Storage::getAssign() const {
    return assignManager->getAllEntitiesEntriesVector();
}

std::vector<std::string> Storage::getConstant() const {
    return constantManager->getAllEntitiesEntriesVector();
}

std::vector<int> Storage::getIf() const {
    return ifManager->getAllEntitiesEntriesVector();
}

std::vector<int> Storage::getPrintS() const {
    return printSManager->getAllEntitiesEntriesVector();
}

std::vector<std::string> Storage::getProcedure() const {
    return procedureManager->getAllEntitiesEntriesVector();
}

std::vector<int> Storage::getReadS() const {
    return readSManager->getAllEntitiesEntriesVector();
}

std::vector<int> Storage::getStmt() const {
    return stmtManager->getAllEntitiesEntriesVector();
}

std::vector<std::string> Storage::getVariable() const {
    return variableManager->getAllEntitiesEntriesVector();
}

std::vector<int> Storage::getWhile() const {
    return whileManager->getAllEntitiesEntriesVector();
}

std::vector<int> Storage::getAssignPatternLhs(std::string& varOrString) const {
    if (assignPatternManager->containsLhsExpr(varOrString)) {
        return assignPatternManager->getLhsValues(std::move(varOrString));
    }
    return std::vector<int>{};
}

std::vector<int> Storage::getIfPattern(std::string& var) const {
    return ifPatternManager->findKeysWithValue(var);
}

std::vector<int> Storage::getIfPatternStmt() const {
    return ifPatternManager->getAllKeys();
}

std::vector<int> Storage::getWhilePatternStmt() const {
    return whilePatternManager->getAllKeys();
}

void Storage::clearIfPattern() const {
    return ifPatternManager->clearCache();
}

std::vector<int> Storage::getWhilePattern(std::string& var) const {
    return whilePatternManager->findKeysWithValue(var);
}

void Storage::clearWhilePattern() const {
    return whilePatternManager->clearCache();
}

std::vector<int> Storage::getAssignPatternRhs(ASTNode& expr) const {
    return assignPatternManager->getRhsValues(expr, false);
}

std::vector<int> Storage::getAssignPatternRhsFull(ASTNode& expr) const {
    return assignPatternManager->getRhsValues(expr, true);
}

std::vector<int> Storage::getAssignPatternFull(std::string& lhs, ASTNode& rhs) const {
    return assignPatternManager->getAllMatch(lhs, rhs, true);
}

std::vector<int> Storage::getAssignPattern(std::string& lhs, ASTNode& rhs) const {
    return assignPatternManager->getAllMatch(lhs, rhs, false);
}

void Storage::storeAssignPattern(const std::string& lhs, const ASTNode& rhs, int stmtNum) {
    assignPatternManager->insertPattern(lhs, rhs, stmtNum);
}

void Storage::storeIfPattern(const std::vector<std::string>& variables, int stmtNum) {
    ifPatternManager->insertRelationship(stmtNum, variables);
}

void Storage::storeWhilePattern(const std::vector<std::string>& variables, int stmtNum) {
    whilePatternManager->insertRelationship(stmtNum, variables);
}

void Storage::storeAssign(int stmtNum) {
    assignManager->insertEntity(stmtNum);
}

void Storage::storeConstant(std::string value) {
    constantManager->insertEntity(std::move(value));
}

void Storage::storeIf(int stmtNum) {
    ifManager->insertEntity(stmtNum);
}

void Storage::storePrintStmt(int stmtNum, std::string& varName) {
    printSManager->insertEntity(stmtNum);
    variableManager->insertEntity(varName);
    printVarNameManager->insertRelationship(stmtNum, varName);
}

void Storage::storeProcedureName(std::string& procName) {
    procedureManager->insertEntity(procName);
}

void Storage::storeReadStmt(int stmtNum, std::string& varName) {
    readSManager->insertEntity(stmtNum);
    variableManager->insertEntity(varName);
    readVarNameManager->insertRelationship(stmtNum, varName);
}

void Storage::storeStmt(int stmtNum) {
    stmtManager->insertEntity(stmtNum);
}

void Storage::storeVariable(std::string& varName) {
    variableManager->insertEntity(varName);
}

void Storage::storeWhile(int stmtNum) {
    whileManager->insertEntity(stmtNum);
}

std::vector<std::string> Storage::extractStringFromChild(const std::vector<ASTNode>& nodes) const {
    std::vector<std::string> stmtValues;
    stmtValues.reserve(nodes.size());
    for (const ASTNode& node : nodes) {
        stmtValues.push_back(node.getValue());
    }
    return stmtValues;
}

std::vector<int> Storage::extractIntFromChild(const std::vector<ASTNode>& nodes) const {
    std::vector<int> stmtValues;
    stmtValues.reserve(nodes.size());
    for (const ASTNode& node : nodes) {
        stmtValues.push_back(node.getStmtNum());
    }
    return stmtValues;
}

void Storage::storeFollows(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    std::vector<int> stmtNums = extractIntFromChild(childrenNodes);
    followsManager->insertRelationship(firstNode.getStmtNum(), stmtNums);
}

void Storage::storeParents(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    std::vector<int> stmtNums = extractIntFromChild(childrenNodes);
    parentManager->insertRelationship(firstNode.getStmtNum(), stmtNums);
}

void Storage::storeParentStar(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    std::vector<int> stmtNums = extractIntFromChild(childrenNodes);
    parentStarManager->insertRelationship(firstNode.getStmtNum(), stmtNums);
}

void Storage::storeUsesS(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    std::vector<std::string> stmtValues = extractStringFromChild(childrenNodes);
    usesSManager->insertRelationship(firstNode.getStmtNum(), stmtValues);
}

void Storage::storeModifiesS(const ASTNode& firstNode, const std::vector<ASTNode>& childrenNodes) {
    std::vector<std::string> stmtValues = extractStringFromChild(childrenNodes);
    modifiesSManager->insertRelationship(firstNode.getStmtNum(), stmtValues);
}

void Storage::storeStmtToProcName(const int stmtNum, std::string& procName) {
    stmtToProcNameManager->insertRelationship(stmtNum, procName);
}

std::string Storage::getStmtToProcName(const int stmtNum) const {
    return getStringFromIntStringMap(stmtToProcNameManager, stmtNum);
}

std::vector<int> Storage::getAllCallStmtToProcName() const {
    return stmtToProcNameManager->getAllKeys();
}

bool Storage::hasCallVarName() const {
    return stmtToProcNameManager->hasKeyValues();
}

void Storage::storeUsesProc(const ASTNode&

                                headNode,
                            const std::vector<ASTNode>& childrenNodes) {
    std::vector<std::string> stmtValues = extractStringFromChild(childrenNodes);
    usesProcManager->insertRelationship(headNode.getValue(), stmtValues);
}

std::vector<std::string> Storage::getUsesProc(const std::string value) {
    return getRsFromManagerStringString(usesProcManager, value);
}

bool Storage::getUsesProc(const std::string& key, const std::string& value) {
    std::vector<std::string> temp = getUsesProc(key);
    return isStringInVector(temp, value);
}

void Storage::storeModifiesProc(const ASTNode& headNode, const std::vector<ASTNode>& childrenNodes) {
    std::vector<std::string> stmtValues = extractStringFromChild(childrenNodes);
    modifiesProcManager->insertRelationship(headNode.getValue(), stmtValues);
}

std::vector<std::string> Storage::getModifiesProc(const std::string value) const {
    return getRsFromManagerStringString(modifiesProcManager, value);
}

bool Storage::getModifiesProc(const std::string& key, const std::string& value) const {
    std::vector<std::string> temp = getModifiesProc(key);
    return isStringInVector(temp, value);
}

void Storage::storeCalls(const std::string& headNode, const std::vector<std::string>& childrenNodes) {
    callsManager->insertRelationship(headNode, childrenNodes);
}

void Storage::storeCallsStar(const std::string& headNode, const std::vector<std::string>& childrenNodes) {
    callsStarManager->insertRelationship(headNode, childrenNodes);
}

std::vector<std::string> Storage::getCalls(const std::string& key) {
    return getRsFromManagerStringString(callsManager, key);
}

std::vector<std::string> Storage::getCallsStar(const std::string& key) {
    return getRsFromManagerStringString(callsStarManager, key);
}

bool Storage::hasCalls() {
    return callsManager->hasKeyValues();
}

bool Storage::hasCallsStar() {
    return callsStarManager->hasKeyValues();
}

std::vector<std::string> Storage::getCallsS2(const std::string& value) {
    return callsManager->findKeysWithValue(value);
}

void Storage::clearCallsS2() const {
    callsManager->clearCache();
    callsStarManager->clearCache();
}

std::vector<std::string> Storage::getCallsStarS2(const std::string& value) {
    return callsStarManager->findKeysWithValue(value);
}

void Storage::storePrintVarName(const int stmtNum, std::string& name) {
    printVarNameManager->insertRelationship(stmtNum, name);
}

std::string Storage::getPrintVarName(const int stmtNum) const {
    return getStringFromIntStringMap(printVarNameManager, stmtNum);
}

void Storage::storeReadVarName(const int stmtNum, std::string& name) {
    readVarNameManager->insertRelationship(stmtNum, name);
}

std::string Storage::getReadVarName(const int stmtNum) const {
    return getStringFromIntStringMap(readVarNameManager, stmtNum);
}

void Storage::dfsReverse(const int& stmt, const std::string& v, std::unordered_set<int>& result,
                         std::unordered_set<int>& visited) const {
    std::vector<int> nextStatements = getNextS2(stmt);
    for (const int& nextStmt : nextStatements) {
        if (visited.find(nextStmt) == visited.end()) {
            visited.insert(nextStmt);
            std::string procName = getStmtToProcName(nextStmt);
            if (!procName.empty() && (affectsManager->containsModifiesProc(procName, v) || getModifiesProc(procName, v))) {
                // call procName and if v is modified in procName
                affectsManager->insertModifiesProcCache(procName, v);
                continue;
            }
            if (assignManager->containsKey(nextStmt) && getModifies(nextStmt, v)) {
                result.insert(nextStmt);
                continue;
            }
            dfsReverse(nextStmt, v, result, visited);
        }
    }
}

void Storage::dfs(const int& stmt, const std::string& v, std::vector<int>& result,
                  std::unordered_set<int>& visited) const {
    std::vector<int> nextStatements = getNext(stmt);
    for (const int& nextStmt : nextStatements) {
        if (visited.find(nextStmt) == visited.end()) {
            visited.insert(nextStmt);
            std::string procName = getStmtToProcName(nextStmt);
            if (!procName.empty() && (affectsManager->containsModifiesProc(procName, v) || getModifiesProc(procName, v))) {
                // call procName and if v is modified in procName
                affectsManager->insertModifiesProcCache(procName, v);
                continue;
            }
            if (affectsManager->containsUses(nextStmt, v) || (assignManager->containsKey(nextStmt) && getUses(nextStmt, v))) {
                result.push_back(nextStmt);
                affectsManager->insertUsesCache(nextStmt, v);
            }
            if ((assignManager->containsKey(nextStmt) || readSManager->containsKey(nextStmt)) && getModifies(nextStmt, v)) {
                continue;
            }
            dfs(nextStmt, v, result, visited);
        }
    }
}

std::vector<int> Storage::getAffects(const int& stmtNum) const {
    std::vector<int> result;
    if (!assignManager->containsKey(stmtNum)) {
        return result;
    }
    result = affectsManager->getAffectedStmt(stmtNum, false);
    std::unordered_set<int> visited;
    std::vector<std::string> varModified = getRsFromManagerIntString(modifiesSManager, stmtNum);  // assumes ea stmt only modifies 1 var
    if (result.empty() && !varModified.empty()) {
        dfs(stmtNum, varModified[0], result, visited);
        affectsManager->insertAffects(stmtNum, result, false);
    }
    return result;
}

std::vector<int> Storage::getAffectsS2(const int& stmtNum) const {
    std::vector<int> result;
    if (!assignManager->containsKey(stmtNum)) {
        return result;
    }
    result = affectsManager->getAffectedStmt(stmtNum, true);
    std::vector<std::string> varUsed = getRsFromManagerIntString(usesSManager, stmtNum);  // assumes ea stmt only modifies 1 var

    if (result.empty()) {
        std::unordered_set<int> temp;
        for (const auto& var : varUsed) {
            std::unordered_set<int> visited;
            dfsReverse(stmtNum, var, temp, visited);
        }
        std::vector<int> final(temp.begin(), temp.end());

        affectsManager->insertAffects(stmtNum, final, true);
        return final;
    }
    return result;
}

bool Storage::hasAffects() const {
    std::unordered_set<int> allAssign = assignManager->getAllEntitiesEntries();
    if (allAssign.empty()) {
        return false;
    }
    if (!affectsManager->getCache().empty())
        return true;
    return std::any_of(allAssign.begin(), allAssign.end(), [&](int a) {
        std::vector<int> result = getAffects(a);
        return !result.empty();
    });
}

void Storage::clearCacheAffects() {
    affectsManager->clearCache();
}

void Storage::storeNext(CFGNode& headNode) {
    std::vector<std::shared_ptr<CFGNode>> temp = headNode.getSuccessors();
    std::vector<int> stmtValues;
    stmtValues.reserve(temp.size());
    for (const std::shared_ptr<CFGNode>& nodePtr : temp) {
        if (nodePtr) {  // Check if the pointer is not null
            int stmtNum = nodePtr->getStmtNum();
            if (stmtNum > 0) {  // Check if the statement number is greater than 0 before storing
                stmtValues.push_back(stmtNum);
            }
        }
    }
    if (stmtValues.size() > 0) {
        nextManager->insertNext(headNode.getStmtNum(), stmtValues);
    }
}

std::vector<int> Storage::getNext(const int& stmtNum) const {
    return nextManager->getNext(stmtNum);
    try {
        return nextManager->getNext(stmtNum);
    } catch (const std::out_of_range& e) {
        return std::vector<int>();
    }
}

std::vector<int> Storage::getNextStar(const int& stmtNum) const {
    return nextManager->findAllReachableNodes(stmtNum, false);
}

std::vector<int> Storage::getNextS2(const int& stmtNum) const {
    return nextManager->getNextS2(stmtNum);
}

std::vector<int> Storage::getNextStarS2(const int& stmtNum) const {
    return nextManager->findAllReachableNodes(stmtNum, true);
}

bool Storage::hasNext() const {
    return nextManager->notEmpty();
}

bool Storage::hasNextStar() {
    return nextManager->hasNextStar();
}

void Storage::clearCacheNext() const {
    nextManager->clearCache();
}

void Storage::clearParents() const {
    parentManager->clearCache();
    parentStarManager->clearCache();
}

void Storage::clearFollows() const {
    followsManager->clearCache();
}