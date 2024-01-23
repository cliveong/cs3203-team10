#pragma once

#include "StmtStmtClauseEvaluator.h"

class FollowsClauseEvaluator : public StmtStmtClauseEvaluator {
   public:
    FollowsClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
