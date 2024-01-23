#include "StmtStmtClauseEvaluator.h"

#include "qps/Utils.h"

StmtStmtClauseEvaluator::StmtStmtClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                                 std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ClauseEvaluator(resultsTable, readFacade, clause) {}

bool StmtStmtClauseEvaluator::evaluateWildcardWildcard() {
    return clause->isNot() ^ getAllStmts();
}

bool StmtStmtClauseEvaluator::evaluateWildcardInteger() {
    return clause->isNot() ^ !getStmtsSecondArg(std::stoi(clause->getSecondArg()->getValue())).empty();
}

bool StmtStmtClauseEvaluator::evaluateWildcardSynonym() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& stmtNum : stmtNumList) {
        if (clause->isNot() ^ !getStmtsSecondArg(stmtNum).empty()) {
            results.push_back(std::to_string(stmtNum));
        }
    }
    return resultsTable->addSingleResult(clause->getSecondArg()->getSynonym()->getSynonymName(), results);
}

bool StmtStmtClauseEvaluator::evaluateIntegerWildcard() {
    return clause->isNot() ^ !getStmtsFirstArg(std::stoi(clause->getFirstArg()->getValue())).empty();
}

bool StmtStmtClauseEvaluator::evaluateIntegerInteger() {
    std::vector<int> firstArgResults = getStmtsFirstArg(std::stoi(clause->getFirstArg()->getValue()));
    return clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), std::stoi(clause->getSecondArg()->getValue())) != firstArgResults.end();
}

bool StmtStmtClauseEvaluator::evaluateIntegerSynonym() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    std::vector<int> firstArgResults = getStmtsFirstArg(std::stoi(clause->getFirstArg()->getValue()));
    for (auto& stmtNum : stmtNumList) {
        if (clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), stmtNum) != firstArgResults.end()) {
            results.push_back(std::to_string(stmtNum));
        }
    }
    return resultsTable->addSingleResult(clause->getSecondArg()->getSynonym()->getSynonymName(), results);
}

bool StmtStmtClauseEvaluator::evaluateSynonymWildcard() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& stmtNum : stmtNumList) {
        if (clause->isNot() ^ !getStmtsFirstArg(stmtNum).empty()) {
            results.push_back(std::to_string(stmtNum));
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool StmtStmtClauseEvaluator::evaluateSynonymInteger() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<int> secondArgResults = getStmtsSecondArg(std::stoi(clause->getSecondArg()->getValue()));
    std::vector<std::string> results;
    for (auto& stmtNum : stmtNumList) {
        if (clause->isNot() ^ std::find(secondArgResults.begin(), secondArgResults.end(), stmtNum) != secondArgResults.end()) {
            results.push_back(std::to_string(stmtNum));
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool StmtStmtClauseEvaluator::evaluateSynonymSynonym() {
    if (*(clause->getFirstArg()) == *(clause->getSecondArg())) {
        std::vector<int> stmtNumList = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
        std::vector<std::string> results;
        for (auto& s1Num : stmtNumList) {
            std::vector<int> firstArgResults = getStmtsFirstArg(s1Num);
            if (clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), s1Num) != firstArgResults.end()) {
                results.push_back(std::to_string(s1Num));
            }
        }
        return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
    }
    std::vector<int> stmtNumList1 = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<int> stmtNumList2 = Utils::getStmtList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results;
    for (auto& s1Num : stmtNumList1) {
        std::vector<int> firstArgResults = getStmtsFirstArg(s1Num);
        for (auto& s2Num : stmtNumList2) {
            if (clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), s2Num) != firstArgResults.end()) {
                results.push_back({std::to_string(s1Num), std::to_string(s2Num)});
            }
        }
    }
    return resultsTable->addPairResult({clause->getFirstArg()->getSynonym()->getSynonymName(), clause->getSecondArg()->getSynonym()->getSynonymName()}, results);
}

bool StmtStmtClauseEvaluator::evaluateClause() {
    std::shared_ptr<Argument> firstArg = clause->getFirstArg();
    std::shared_ptr<Argument> secondArg = clause->getSecondArg();

    if (firstArg->getType() == WILDCARD) {
        if (secondArg->getType() == WILDCARD) {
            return evaluateWildcardWildcard();
        } else if (secondArg->getType() == INT_ARG) {
            return evaluateWildcardInteger();
        } else if (secondArg->getType() == SYNONYM) {
            return evaluateWildcardSynonym();
        }
    } else if (firstArg->getType() == INT_ARG) {
        if (secondArg->getType() == WILDCARD) {
            return evaluateIntegerWildcard();
        } else if (secondArg->getType() == INT_ARG) {
            return evaluateIntegerInteger();
        } else if (secondArg->getType() == SYNONYM) {
            return evaluateIntegerSynonym();
        }
    } else if (firstArg->getType() == SYNONYM) {
        if (secondArg->getType() == WILDCARD) {
            return evaluateSynonymWildcard();
        } else if (secondArg->getType() == INT_ARG) {
            return evaluateSynonymInteger();
        } else if (secondArg->getType() == SYNONYM) {
            return evaluateSynonymSynonym();
        }
    }
    return true;
}
