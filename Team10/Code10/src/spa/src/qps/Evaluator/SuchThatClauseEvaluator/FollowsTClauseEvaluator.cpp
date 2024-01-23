#include "FollowsTClauseEvaluator.h"

FollowsTClauseEvaluator::FollowsTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                                 std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getFollows(stmt, true); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getFollowsS2(stmt, true); };
    getAllStmts = [readFacade]() { return readFacade->hasFollowsStar(); };
}
