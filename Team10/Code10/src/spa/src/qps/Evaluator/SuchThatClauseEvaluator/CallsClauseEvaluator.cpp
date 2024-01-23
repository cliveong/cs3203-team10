#include "CallsClauseEvaluator.h"

CallsClauseEvaluator::CallsClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable,
                                           std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause) : ProcedureProcedureClauseEvaluator(resultsTable, readFacade, clause) {
    getProcedureFirstArg = [readFacade](const std::string& procedure) { return readFacade->getCalls(procedure, false); };
    getProcedureSecondArg = [readFacade](const std::string& procedure) { return readFacade->getCallsS2(procedure, false); };
    getAllProcedures = [readFacade]() { return readFacade->hasCalls(false); };
}
