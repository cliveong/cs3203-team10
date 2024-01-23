#pragma once

#include "StmtStmtClauseEvaluator.h"

class ParentClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    ParentClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
