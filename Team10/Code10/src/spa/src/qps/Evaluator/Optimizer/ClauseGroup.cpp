#include "ClauseGroup.h"

#include <utility>

#include "qps/Clause/PatternClause/PatternClause.h"
#include "qps/Evaluator/ClauseEvaluator.h"
#include "qps/Evaluator/ClauseEvaluatorFactory.h"
#include "qps/Utils.h"

ClauseGroup::ClauseGroup(std::shared_ptr<ReadFacade> readFacade) : readFacade(std::move(readFacade)) {}

bool ClauseGroup::operator==(const ClauseGroup& clauseGroup) {
    return this->synonymGroup == clauseGroup.synonymGroup && this->clauseList == clauseGroup.clauseList;
}

bool ClauseGroup::synonymInGroup(const std::string& synonymName) {
    return synonymGroup.find(synonymName) != synonymGroup.end();
}

void ClauseGroup::addClause(const std::shared_ptr<Clause>& clause) {
    std::pair<std::string, std::string> synonyms = Utils::getSynonyms(clause);
    if (!synonyms.first.empty()) {
        synonymGroup.insert(synonyms.first);
    }
    if (!synonyms.second.empty()) {
        synonymGroup.insert(synonyms.second);
    }
    clauseList.push_back(clause);
}

std::vector<std::shared_ptr<Clause>> ClauseGroup::getClauses() {
    return clauseList;
}

std::shared_ptr<ResultsTable> ClauseGroup::getResultsTable() {
    return resultsTable;
}

void ClauseGroup::setClauseList(std::vector<std::shared_ptr<Clause>> newClauseList) {
    clauseList = std::move(newClauseList);
}

bool ClauseGroup::evaluateGroup() {
    ClauseEvaluatorFactory clauseFactory = ClauseEvaluatorFactory();
    for (auto& clause : clauseList) {
        std::shared_ptr<ClauseEvaluator> clauseEvaluator = clauseFactory.getEvaluator(clause, resultsTable, readFacade);
        bool evaluated = clauseEvaluator->evaluateClause();
        if (!evaluated) {
            return false;
        }
    }
    return true;
}

std::pair<std::map<int, std::string>, std::vector<std::vector<std::string>>>
ClauseGroup::getResults(const std::vector<std::shared_ptr<Synonym>>& synonyms) {
    return resultsTable->getSynonymResults(synonyms);
}