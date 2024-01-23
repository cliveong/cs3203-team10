#include "IfPatternClauseEvaluator.h"

IfPatternClauseEvaluator::IfPatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                                   std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : PatternClauseEvaluator(resultsTable, readFacade, clause) {
    getWildcardStmts = [readFacade]() { return readFacade->getIfPatternStmt(); };
    getPatternLHS = [readFacade](std::string variable) { return readFacade->getIfPattern(variable); };
    getPatternRHS = [readFacade](ASTNode astNode, bool isFullMatch) { return std::vector<int>(); };
    getPatternStmts = [readFacade](std::string variable, ASTNode astNode, bool isFullMatch) { return std::vector<int>(); };
}
