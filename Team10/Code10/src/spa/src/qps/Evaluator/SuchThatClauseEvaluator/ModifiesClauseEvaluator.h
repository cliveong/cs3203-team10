#pragma once

#include "StmtEntClauseEvaluator.h"

class ModifiesClauseEvaluator : public StmtEntClauseEvaluator {
   public:
    ModifiesClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
};
