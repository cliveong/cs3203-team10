#include "Query.h"

#include <utility>

Query::Query(std::vector<std::shared_ptr<Synonym>> synonyms, std::shared_ptr<SelectClause> selectClause,
             std::vector<std::shared_ptr<Clause>> constraintClauses) : synonyms(std::move(synonyms)), selectClause(std::move(selectClause)), constraintClauses(std::move(constraintClauses)) {}

std::shared_ptr<SelectClause> Query::getSelectClause() {
    return selectClause;
}

std::vector<std::shared_ptr<Clause>> Query::getConstraintClauses() {
    return constraintClauses;
}

std::vector<std::shared_ptr<Synonym>> Query::getSynonyms() {
    return synonyms;
}

static bool synonymPred(const std::shared_ptr<Synonym>& syn1, const std::shared_ptr<Synonym>& syn2) {
    return (*syn1) == (*syn2);
}

static bool clausePred(const std::shared_ptr<Clause>& clause1, const std::shared_ptr<Clause>& clause2) {
    return (*clause1) == (*clause2);
}

bool Query::equals(const Query& query) {
    bool isSynonymsEqual = std::equal(synonyms.begin(), synonyms.end(),
                                      query.synonyms.begin(), query.synonyms.end(),
                                      synonymPred);

    bool isSelectClauseEqual = (*selectClause) == *(query.selectClause);

    bool isConstraintClausesEqual = std::equal(constraintClauses.begin(), constraintClauses.end(),
                                               query.constraintClauses.begin(), query.constraintClauses.end(),
                                               clausePred);

    return isSynonymsEqual && isSelectClauseEqual && isConstraintClausesEqual;
}
