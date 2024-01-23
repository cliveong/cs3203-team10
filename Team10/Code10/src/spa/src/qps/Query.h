#pragma once

#include <vector>

#include "Clause/Clause.h"
#include "Clause/SelectClause.h"
#include "Synonym.h"

class Query {
   private:
    std::vector<std::shared_ptr<Synonym>> synonyms;
    std::shared_ptr<SelectClause> selectClause;
    std::vector<std::shared_ptr<Clause>> constraintClauses;

   public:
    Query(std::vector<std::shared_ptr<Synonym>> synonymList, std::shared_ptr<SelectClause> selectClause,
          std::vector<std::shared_ptr<Clause>> constraintClauses);
    std::shared_ptr<SelectClause> getSelectClause();
    std::vector<std::shared_ptr<Clause>> getConstraintClauses();
    std::vector<std::shared_ptr<Synonym>> getSynonyms();
    bool equals(const Query& query);
};
