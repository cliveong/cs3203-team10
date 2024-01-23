#include "StmtEntClauseEvaluator.h"

#include "qps/Utils.h"

StmtEntClauseEvaluator::StmtEntClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                               std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ClauseEvaluator(resultsTable, readFacade, clause) {}

bool StmtEntClauseEvaluator::evaluateStmtVariable() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> varList = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results;
    for (auto& stmtNum : stmtNumList) {
        std::vector<std::string> stmtResult = getStmtVariables(stmtNum);
        for (auto& var : varList) {
            if (clause->isNot() ^ std::find(stmtResult.begin(), stmtResult.end(), var) != stmtResult.end()) {
                results.push_back({std::to_string(stmtNum), var});
            }
        }
    }
    return resultsTable->addPairResult({clause->getFirstArg()->getSynonym()->getSynonymName(), clause->getSecondArg()->getSynonym()->getSynonymName()}, results);
}

bool StmtEntClauseEvaluator::evaluateStmtString() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& stmtNum : stmtNumList) {
        if (clause->isNot() ^ checkStmtRelation(stmtNum, clause->getSecondArg()->getValue())) {
            results.push_back(std::to_string(stmtNum));
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool StmtEntClauseEvaluator::evaluateStmtWildcard() {
    std::vector<int> stmtNumList = Utils::getStmtList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& stmtNum : stmtNumList) {
        if (clause->isNot() ^ !getStmtVariables(stmtNum).empty()) {
            results.push_back(std::to_string(stmtNum));
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool StmtEntClauseEvaluator::evaluateIntVariable() {
    std::vector<std::string> varResult = getStmtVariables(std::stoi(clause->getFirstArg()->getValue()));
    std::vector<std::string> varList = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& var : varList) {
        if (clause->isNot() ^ std::find(varResult.begin(), varResult.end(), var) != varResult.end()) {
            results.push_back(var);
        }
    }
    return resultsTable->addSingleResult(clause->getSecondArg()->getSynonym()->getSynonymName(), results);
}

bool StmtEntClauseEvaluator::evaluateIntString() {
    return clause->isNot() ^ checkStmtRelation(std::stoi(clause->getFirstArg()->getValue()), clause->getSecondArg()->getValue());
}

bool StmtEntClauseEvaluator::evaluateIntWildcard() {
    std::vector<std::string> varResult = getStmtVariables(std::stoi(clause->getFirstArg()->getValue()));
    return clause->isNot() ^ !varResult.empty();
}

bool StmtEntClauseEvaluator::evaluateStringVariable() {
    std::vector<std::string> varResult = getProcVariables(clause->getFirstArg()->getValue());
    std::vector<std::string> varList = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& var : varList) {
        if (clause->isNot() ^ std::find(varResult.begin(), varResult.end(), var) != varResult.end()) {
            results.push_back(var);
        }
    }
    return resultsTable->addSingleResult(clause->getSecondArg()->getSynonym()->getSynonymName(), results);
}

bool StmtEntClauseEvaluator::evaluateStringString() {
    return clause->isNot() ^ checkProcRelation(clause->getFirstArg()->getValue(), clause->getSecondArg()->getValue());
}

bool StmtEntClauseEvaluator::evaluateStringWildcard() {
    return clause->isNot() ^ !getProcVariables(clause->getFirstArg()->getValue()).empty();
}

bool StmtEntClauseEvaluator::evaluateProcedureVariable() {
    std::vector<std::string> procedures = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> varList = Utils::getEntitiesList(clause->getSecondArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results;
    for (auto& procedure : procedures) {
        std::vector<std::string> procedureVar = getProcVariables(procedure);
        for (auto& var : varList) {
            if (clause->isNot() ^ std::find(procedureVar.begin(), procedureVar.end(), var) != procedureVar.end()) {
                results.push_back({procedure, var});
            }
        }
    }
    return resultsTable->addPairResult({clause->getFirstArg()->getSynonym()->getSynonymName(), clause->getSecondArg()->getSynonym()->getSynonymName()}, results);
}

bool StmtEntClauseEvaluator::evaluateProcedureString() {
    std::vector<std::string> procedures = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::string var = clause->getSecondArg()->getValue();
    std::vector<std::string> results;
    for (auto& procedure : procedures) {
        if (clause->isNot() ^ checkProcRelation(procedure, var)) {
            results.push_back(procedure);
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool StmtEntClauseEvaluator::evaluateProcedureWildcard() {
    std::vector<std::string> procedures = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& procedure : procedures) {
        if (clause->isNot() ^ !getProcVariables(procedure).empty()) {
            results.push_back(procedure);
        }
    }
    return resultsTable->addSingleResult(clause->getFirstArg()->getSynonym()->getSynonymName(), results);
}

bool StmtEntClauseEvaluator::evaluateClause() {
    std::shared_ptr<Argument> firstArg = clause->getFirstArg();
    std::shared_ptr<Argument> secondArg = clause->getSecondArg();
    if (Utils::isStatement(firstArg)) {
        std::vector<int> stmtNumList = Utils::getStmtList(firstArg->getSynonym(), readFacade, resultsTable);
        if (secondArg->getType() == SYNONYM) {
            return evaluateStmtVariable();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateStmtString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateStmtWildcard();
        }
    } else if (firstArg->getType() == INT_ARG) {
        if (secondArg->getType() == SYNONYM) {
            return evaluateIntVariable();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateIntString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateIntWildcard();
        }
    } else if (firstArg->getType() == STRING_ARG) {
        if (secondArg->getType() == SYNONYM) {
            return evaluateStringVariable();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateStringString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateStringWildcard();
        }
    } else if (firstArg->getType() == SYNONYM && firstArg->getSynonym()->getSynonymType() == PROCEDURE) {
        if (secondArg->getType() == SYNONYM) {
            return evaluateProcedureVariable();
        } else if (secondArg->getType() == STRING_ARG) {
            return evaluateProcedureString();
        } else if (secondArg->getType() == WILDCARD) {
            return evaluateProcedureWildcard();
        }
    }
    return true;
}
