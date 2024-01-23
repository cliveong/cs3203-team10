#include "ModifiesClauseEvaluator.h"

ModifiesClauseEvaluator::ModifiesClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                                 std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : StmtEntClauseEvaluator(resultsTable, readFacade, clause) {
    getStmtVariables = [readFacade](int stmt) { return readFacade->getModifies(stmt); };
    checkStmtRelation = [readFacade](int stmt, std::string var) { return readFacade->getModifies(stmt, var); };
    getProcVariables = [readFacade](std::string proc) { return readFacade->getModifiesProc(proc); };
    checkProcRelation = [readFacade](std::string proc, std::string var) { return readFacade->getModifiesProc(proc, var); };
}
