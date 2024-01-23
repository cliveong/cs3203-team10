#include "CallsTClauseEvaluator.h"

CallsTClauseEvaluator::CallsTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                             std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ProcedureProcedureClauseEvaluator(resultsTable, readFacade, clause) {
    getProcedureFirstArg = [readFacade](const std::string& procedure) { return readFacade->getCalls(procedure, true); };
    getProcedureSecondArg = [readFacade](const std::string& procedure) { return readFacade->getCallsS2(procedure, true); };
    getAllProcedures = [readFacade]() { return readFacade->hasCalls(true); };
}