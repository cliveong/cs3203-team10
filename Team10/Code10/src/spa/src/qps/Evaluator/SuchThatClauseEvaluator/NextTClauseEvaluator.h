#pragma once

#include "StmtStmtClauseEvaluator.h"

class NextTClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    NextTClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
