#include "AffectsClauseEvaluator.h"

AffectsClauseEvaluator::AffectsClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                               std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtStmtClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtsFirstArg = [readFacade](int stmt) { return readFacade->getAffects(stmt); };
    getStmtsSecondArg = [readFacade](int stmt) { return readFacade->getAffectsS2(stmt); };
    getAllStmts = [readFacade]() { return readFacade->hasAffects(); };
}
