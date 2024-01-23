#include "ParentClauseEvaluator.h"

ParentClauseEvaluator::ParentClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                             std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getParents(stmt, false); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getParentsS2(stmt, false); };
    getAllStmts = [readFacade]() { return readFacade->hasParents(false); };
}
