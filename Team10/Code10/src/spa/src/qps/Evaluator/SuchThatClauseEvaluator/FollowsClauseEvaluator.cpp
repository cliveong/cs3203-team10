#include "FollowsClauseEvaluator.h"

FollowsClauseEvaluator::FollowsClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                               std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getFollows(stmt, false); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getFollowsS2(stmt, false); };
    getAllStmts = [readFacade]() { return readFacade->hasFollows(); };
}
