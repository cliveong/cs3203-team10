#pragma once
#include "ProcedureProcedureClauseEvaluator.h"

class CallsTClauseEvaluator : public ProcedureProcedureClauseEvaluator {
   public:
    CallsTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};