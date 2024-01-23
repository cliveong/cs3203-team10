#include "UsesClauseEvaluator.h"

UsesClauseEvaluator::UsesClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                         std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtEntClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtVariables = [readFacade](int stmt) { return readFacade->getUses(stmt); };
    checkStmtRelation = [readFacade](int stmt, std::string var) { return readFacade->getUses(stmt, var); };
    getProcVariables = [readFacade](std::string proc) { return readFacade->getUsesProc(proc); };
    checkProcRelation = [readFacade](std::string proc, std::string var) { return readFacade->getUsesProc(proc, var); };
}
