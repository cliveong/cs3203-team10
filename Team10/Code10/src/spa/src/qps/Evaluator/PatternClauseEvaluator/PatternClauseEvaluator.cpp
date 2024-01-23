#include "PatternClauseEvaluator.h"

#include "commons/ASTNode.h"
#include "commons/ExprNode.h"
#include "qps/Utils.h"
#include "sp/Parsers/ParserContext.h"

PatternClauseEvaluator::PatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                               std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ClauseEvaluator(resultsTable, readFacade, clause) {}

bool PatternClauseEvaluator::evaluateWildcardWildcard(const std::shared_ptr<PatternClause>& patternClause) {
    std::vector<std::string> stmtList = Utils::getEntitiesList(patternClause->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    std::vector<int> stmtResults = getWildcardStmts();
    for (auto& stmt : stmtList) {
        if (clause->isNot() ^ std::find(stmtResults.begin(), stmtResults.end(), std::stoi(stmt)) != stmtResults.end()) {
            results.push_back(stmt);
        }
    }
    return resultsTable->addSingleResult(patternClause->getSynonym()->getSynonymName(), results);
}

bool PatternClauseEvaluator::evaluateWildcardPattern(const std::shared_ptr<PatternClause>& patternClause) {
    std::vector<int> assignList = Utils::getStmtList(patternClause->getSynonym(), readFacade, resultsTable);
    std::shared_ptr<ExprArgument> patternArg = std::dynamic_pointer_cast<ExprArgument>(patternClause->getSecondArg());
    std::shared_ptr<ExprNode> exprAST = patternArg->getExprAST();
    std::vector<int> stmtResults = getPatternRHS(*exprAST, patternArg->isFullMatch());
    std::vector<std::string> results;
    for (auto& a : assignList) {
        if (clause->isNot() ^ std::find(stmtResults.begin(), stmtResults.end(), a) != stmtResults.end()) {
            results.push_back(std::to_string(a));
        }
    }
    return resultsTable->addSingleResult(patternClause->getSynonym()->getSynonymName(), results);
}

bool PatternClauseEvaluator::evaluateStringWildcard(const std::shared_ptr<PatternClause>& patternClause) {
    std::vector<int> stmtResults = getPatternLHS(clause->getFirstArg()->getValue());
    std::vector<int> assignList = Utils::getStmtList(patternClause->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> results;
    for (auto& a : assignList) {
        if (clause->isNot() ^ std::find(stmtResults.begin(), stmtResults.end(), a) != stmtResults.end()) {
            results.push_back(std::to_string(a));
        }
    }
    return resultsTable->addSingleResult(patternClause->getSynonym()->getSynonymName(), results);
}

bool PatternClauseEvaluator::evaluateStringPattern(const std::shared_ptr<PatternClause>& patternClause) {
    std::vector<int> assignList = Utils::getStmtList(patternClause->getSynonym(), readFacade, resultsTable);
    std::shared_ptr<ExprArgument> patternArg = std::dynamic_pointer_cast<ExprArgument>(clause->getSecondArg());
    std::shared_ptr<ExprNode> exprAST = patternArg->getExprAST();
    std::vector<int> stmtResults = getPatternStmts(clause->getFirstArg()->getValue(), *exprAST, patternArg->isFullMatch());
    std::vector<std::string> results;
    for (auto& a : assignList) {
        if (clause->isNot() ^ std::find(stmtResults.begin(), stmtResults.end(), a) != stmtResults.end()) {
            results.push_back(std::to_string(a));
        }
    }
    return resultsTable->addSingleResult(patternClause->getSynonym()->getSynonymName(), results);
}

bool PatternClauseEvaluator::evaluateVariableWildcard(const std::shared_ptr<PatternClause>& patternClause) {
    std::vector<int> assignList = Utils::getStmtList(patternClause->getSynonym(), readFacade, resultsTable);
    std::vector<std::string> variables = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results;
    for (auto& var : variables) {
        std::vector<int> stmtResults = getPatternLHS(var);
        for (auto& a : assignList) {
            if (clause->isNot() ^ std::find(stmtResults.begin(), stmtResults.end(), a) != stmtResults.end()) {
                results.push_back({std::to_string(a), var});
            }
        }
    }
    return resultsTable->addPairResult({patternClause->getSynonym()->getSynonymName(), clause->getFirstArg()->getSynonym()->getSynonymName()}, results);
}

bool PatternClauseEvaluator::evaluateVariablePattern(const std::shared_ptr<PatternClause>& patternClause) {
    std::vector<int> assignList = Utils::getStmtList(patternClause->getSynonym(), readFacade, resultsTable);
    std::shared_ptr<ExprArgument> patternArg = std::dynamic_pointer_cast<ExprArgument>(clause->getSecondArg());
    std::vector<std::string> variables = Utils::getEntitiesList(clause->getFirstArg()->getSynonym(), readFacade, resultsTable);
    std::vector<std::vector<std::string>> results;
    std::shared_ptr<ExprNode> exprAST = patternArg->getExprAST();
    for (auto& var : variables) {
        std::vector<int> stmtResults = getPatternStmts(var, *exprAST, patternArg->isFullMatch());
        for (auto& a : assignList) {
            if (clause->isNot() ^ std::find(stmtResults.begin(), stmtResults.end(), a) != stmtResults.end()) {
                results.push_back({std::to_string(a), var});
            }
        }
    }
    return resultsTable->addPairResult({patternClause->getSynonym()->getSynonymName(), clause->getFirstArg()->getSynonym()->getSynonymName()}, results);
}

bool PatternClauseEvaluator::evaluateClause() {
    std::shared_ptr<PatternClause> patternClause = std::dynamic_pointer_cast<PatternClause>(clause);
    std::shared_ptr<Argument> firstArg = patternClause->getFirstArg();
    std::shared_ptr<Argument> secondArg = patternClause->getSecondArg();

    if (secondArg->getType() == WILDCARD) {
        if (firstArg->getType() == WILDCARD) {
            return evaluateWildcardWildcard(patternClause);
        } else if (firstArg->getType() == STRING_ARG) {
            return evaluateStringWildcard(patternClause);
        } else if (firstArg->getType() == SYNONYM) {
            return evaluateVariableWildcard(patternClause);
        }
    } else if (secondArg->getType() == EXPRESSION) {
        std::shared_ptr<ExprArgument> patternArg = std::dynamic_pointer_cast<ExprArgument>(secondArg);
        if (firstArg->getType() == WILDCARD) {
            return evaluateWildcardPattern(patternClause);
        } else if (firstArg->getType() == STRING_ARG) {
            return evaluateStringPattern(patternClause);
        } else if (firstArg->getType() == SYNONYM) {
            return evaluateVariablePattern(patternClause);
        }
    }

    return true;
}