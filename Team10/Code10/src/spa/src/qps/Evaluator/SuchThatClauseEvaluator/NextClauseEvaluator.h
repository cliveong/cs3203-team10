#pragma once

#include "StmtStmtClauseEvaluator.h"

class NextClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    NextClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
