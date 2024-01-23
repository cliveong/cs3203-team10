#include "AssignPatternClauseEvaluator.h"

AssignPatternClauseEvaluator::AssignPatternClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                                           std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : PatternClauseEvaluator(resultsTable, readFacade, clause) {
    getWildcardStmts = [readFacade]() { return readFacade->getAssign(); };
    getPatternLHS = [readFacade](std::string variable) { return readFacade->getAssignPatternLhs(variable); };
    getPatternRHS = [readFacade](ASTNode astNode, bool isFullMatch) { return readFacade->getAssignPatternRhs(astNode, isFullMatch); };
    getPatternStmts = [readFacade](std::string variable, ASTNode astNode, bool isFullMatch) { return readFacade->getAssignPattern(variable, astNode, isFullMatch); };
}
