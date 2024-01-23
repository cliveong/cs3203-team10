#include "ParentTClauseEvaluator.h"

ParentTClauseEvaluator::ParentTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                               std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getParents(stmt, true); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getParentsS2(stmt, true); };
    getAllStmts = [readFacade]() { return readFacade->hasParents(true); };
}
