#pragma once

#include "ConstraintClauseParser.h"
#include "qps/Validator/SyntaxValidator.h"

class SuchThatClauseParser : public ConstraintClauseParser {
   private:
    // contains info of all clauses (connected by `and`) being parsed
    std::vector<struct clauseInfo> allClauseInfo;

   public:
    SuchThatClauseParser();
    explicit SuchThatClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseClause(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) override;
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseRelCond(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseRelRef(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<struct clauseInfo> getClauseInfo();
};
