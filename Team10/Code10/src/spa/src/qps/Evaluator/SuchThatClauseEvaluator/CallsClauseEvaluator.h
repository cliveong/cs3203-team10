#pragma once
#include "ProcedureProcedureClauseEvaluator.h"

class CallsClauseEvaluator : public ProcedureProcedureClauseEvaluator {
   public:
    CallsClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
