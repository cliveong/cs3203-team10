#include "NextClauseEvaluator.h"

NextClauseEvaluator::NextClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                         std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getNext(stmt, false); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getNextS2(stmt, false); };
    getAllStmts = [readFacade]() { return readFacade->hasNext(); };
}
