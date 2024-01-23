#include "ClauseEvaluator.h"

#include <utility>

ClauseEvaluator::ClauseEvaluator(std::shared_ptr<ResultsTable> resultsTable, std::shared_ptr<ReadFacade> readFacade, std::shared_ptr<Clause> clause)
    : readFacade(std::move(readFacade)), resultsTable(std::move(resultsTable)), clause(std::move(clause)){};

bool ClauseEvaluator::evaluateClause() {
    return false;
}
