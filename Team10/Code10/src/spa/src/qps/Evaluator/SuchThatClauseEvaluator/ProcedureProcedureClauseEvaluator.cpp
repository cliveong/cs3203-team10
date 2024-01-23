#include "ProcedureProcedureClauseEvaluator.h"

#include "qps/Utils.h"

ProcedureProcedureClauseEvaluator::ProcedureProcedureClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                                                     std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ClauseEvaluator(resultsTable, readFacade, clause) {}

bool ProcedureProcedureClauseEvaluator::evaluateProcedureProcedure() {
    if (*(clause->getFirstArg()) == *(clause->getSecondArg())) {
        if (clause->isNot()) {
            resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable));
        } else {
            return false;
        }
    }
    std::vector<std::string> procedureList1 = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> procedureList2 = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results;
    for (auto& procedure1 : procedureList1) {
        std::vector<std::string> firstArgResults = getProcedureFirstArg(procedure1);
        for (auto& procedure2 : procedureList2) {
            if (clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), procedure2) != firstArgResults.end()) {
                results.push_back({procedure1, procedure2});
            }
        }
    }
    return resultsTable->addPairResult({clause->getFirstArg()->getSynonym()->getSynonymName(), clause->getSecondArg()->getSynonym()->getSynonymName()}, results);
}

bool ProcedureProcedureClauseEvaluator::evaluateProcedureString() {
    std::vector<std::string> procedureList = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> secondArgResults = getProcedureSecondArg(clause->getSecondArg()->getValue());
    std::vector<std::string> results;
    for (auto& procedure : procedureList) {
        if (clause->isNot() ^ std::find(secondArgResults.begin(), secondArgResults.end(), procedure) != secondArgResults.end()) {
            results.push_back(procedure);
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool ProcedureProcedureClauseEvaluator::evaluateProcedureWildcard() {
    std::vector<std::string> procedureList = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& procedure : procedureList) {
        if (clause->isNot() ^ !getProcedureFirstArg(procedure).empty()) {
            results.push_back(procedure);
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool ProcedureProcedureClauseEvaluator::evaluateStringProcedure() {
    std::vector<std::string> firstArgResults = getProcedureFirstArg(clause->getFirstArg()->getValue());
    std::vector<std::string> procedureList = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& procedure : procedureList) {
        if (clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), procedure) != firstArgResults.end()) {
            results.push_back(procedure);
        }
    }
    return resultsTable->addSingleResult(clause->getSecondArg()->getSynonym()->getSynonymName(), results);
}

bool ProcedureProcedureClauseEvaluator::evaluateStringString() {
    std::vector<std::string> firstArgResults = getProcedureFirstArg(clause->getFirstArg()->getValue());
    return clause->isNot() ^ std::find(firstArgResults.begin(), firstArgResults.end(), clause->getSecondArg()->getValue()) != firstArgResults.end();
}

bool ProcedureProcedureClauseEvaluator::evaluateStringWildcard() {
    return clause->isNot() ^ !getProcedureFirstArg(clause->getFirstArg()->getValue()).empty();
}

bool ProcedureProcedureClauseEvaluator::evaluateWildcardProcedure() {
    std::vector<std::string> procedureList = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& procedure : procedureList) {
        if (clause->isNot() ^ !getProcedureSecondArg(procedure).empty()) {
            results.push_back(procedure);
        }
    }
    return resultsTable->addSingleResult(clause->getSecondArg()->getSynonym()->getSynonymName(), results);
}

bool ProcedureProcedureClauseEvaluator::evaluateWildcardString() {
    return clause->isNot() ^ !getProcedureSecondArg(clause->getSecondArg()->getValue()).empty();
}

bool ProcedureProcedureClauseEvaluator::evaluateWildcardWildcard() {
    return clause->isNot() ^ getAllProcedures();
}

bool ProcedureProcedureClauseEvaluator::evaluateClause() {
    std::shared_ptr<Argument> firstArg = clause->getFirstArg();
    std::shared_ptr<Argument> secondArg = clause->getSecondArg();
    if (firstArg->isSynonym()) {
        if (secondArg->isSynonym()) {
            return evaluateProcedureProcedure();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateProcedureString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateProcedureWildcard();
        }
    } else if (firstArg->getType() == STRING_ARG) {
        if (secondArg->isSynonym()) {
            return evaluateStringProcedure();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateStringString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateStringWildcard();
        }
    } else if (firstArg->getType() == WILDCARD) {
        if (secondArg->isSynonym()) {
            return evaluateWildcardProcedure();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateWildcardString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateWildcardWildcard();
        }
    }
    return true;
}
