#pragma once

#include <memory>

#include "pkb/ReadFacade.h"
#include "qps/Clause/Clause.h"
#include "qps/ResultsTable.h"

class ClauseEvaluator {
   protected:
    std::shared_ptr<ResultsTable> resultsTable;
    std::shared_ptr<ReadFacade> readFacade;
    std::shared_ptr<Clause> clause;

   public:
    ClauseEvaluator() = default;
    ClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause);
    virtual bool evaluateClause();
};