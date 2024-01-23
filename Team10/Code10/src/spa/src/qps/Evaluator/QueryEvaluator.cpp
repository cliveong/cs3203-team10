#include "QueryEvaluator.h"

#include <utility>
#include "ClauseEvaluator.h"
#include "Optimizer/ClauseOptimizer.h"
#include "SelectClauseEvaluator.h"
#include "qps/AttrRef.h"
#include "qps/Elem.h"
#include "qps/Utils.h"

QueryEvaluator::QueryEvaluator(std::shared_ptr<Query> query, std::shared_ptr<ReadFacade> readFacade) : query(std::move(query)), readFacade(std::move(readFacade)){};

std::vector<std::string> QueryEvaluator::evaluateQuery() {
    // Get Select Clause
    std::shared_ptr<SelectClause> selectClause = query->getSelectClause();
    std::vector<std::shared_ptr<Clause>> constraintClauses = query->getConstraintClauses();

    ClauseOptimizer clauseOptimizer;
    std::vector<std::shared_ptr<ClauseGroup>> clauseGroups = clauseOptimizer.optimizeClauses(constraintClauses, readFacade);
    for (auto& group : clauseGroups) {
        bool evaluated = group->evaluateGroup();
        if (!evaluated) {
            if (selectClause->isReturnBool()) {
                return {"FALSE"};
            }
            return {};  // Empty list if any clause evaluates to false -> Stops evaluation of rest of clauses too
        }
    }

    // If boolean select, return TRUE. (All clauses evaluate to True if for loop exits)
    if (selectClause->isReturnBool()) {
        return {"TRUE"};
    }

    std::vector<std::shared_ptr<Elem>> elemList = selectClause->getElements();
    std::shared_ptr<ResultsTable> resultsTable = getEvaluatedElements(clauseGroups, elemList);
    // Evaluate select clause
    std::shared_ptr<SelectClauseEvaluator> selectEvaluator = std::make_shared<SelectClauseEvaluator>();
    bool evaluated = selectEvaluator->evaluate(selectClause, resultsTable, readFacade);
    if (!evaluated) {
        return {};
    }
    // Get final results
    return resultsTable->getResult(elemList, readFacade);
}

std::shared_ptr<ResultsTable> QueryEvaluator::getEvaluatedElements(
    const std::vector<std::shared_ptr<ClauseGroup>>& clauseGroups, const std::vector<std::shared_ptr<Elem>>& elemList) {
    std::shared_ptr<ResultsTable> resultsTable = std::make_shared<ResultsTable>();
    for (auto& group : clauseGroups) {
        std::unordered_set<std::shared_ptr<Synonym>> synonymsInGroup;
        for (auto& element : elemList) {  // Check if any synonym is in the group
            std::shared_ptr<Synonym> synonym = Utils::getSynonym(element);
            std::string synonymName = synonym->getSynonymName();
            if (group->synonymInGroup(synonymName)) {
                synonymsInGroup.insert(synonym);
            }
        }
        if (synonymsInGroup.empty()) {
            continue;
        }
        // Find results for synonym(s) in group
        std::vector<std::shared_ptr<Synonym>> synonymList;
        synonymList.assign(synonymsInGroup.begin(), synonymsInGroup.end());
        std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>> results = group->getResults(synonymList);
        resultsTable->combineResults(results);
    }

    return resultsTable;
}
