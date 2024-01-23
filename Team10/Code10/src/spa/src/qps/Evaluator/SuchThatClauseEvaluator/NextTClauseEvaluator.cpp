#include "NextTClauseEvaluator.h"

NextTClauseEvaluator::NextTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                           std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getNext(stmt, true); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getNextS2(stmt, true); };
    getAllStmts = [readFacade]() { return readFacade->hasNextStar(); };
}
