#pragma once

#include <algorithm>
#include <unordered_set>
#include <vector>

#include "../Tokenizer/QueryToken.h"
#include "PatternClauseParser.h"
#include "SelectClauseParser.h"
#include "SuchThatClauseParser.h"
#include "WithClauseParser.h"
#include "qps/Clause/Clause.h"
#include "qps/Clause/SelectClause.h"
#include "qps/ExprArgument.h"
#include "qps/Validator/SyntaxValidator.h"

class ClauseParser {
   private:
    std::vector<std::shared_ptr<Synonym>> synonyms;
    std::shared_ptr<SelectClauseParser> selectClauseParser;
    std::shared_ptr<SelectClause> selectClause;
    // Vector containing such that clauses and pattern clauses
    std::vector<std::shared_ptr<Clause>> constraintClauses;
    std::shared_ptr<SyntaxValidator> syntaxValidator;
    std::shared_ptr<SuchThatClauseParser> suchThatClauseParser;
    std::shared_ptr<PatternClauseParser> patternClauseParser;
    std::shared_ptr<WithClauseParser> withClauseParser;

    std::shared_ptr<Synonym> getSynonymInList(const std::string&);
    // Parses constraint clause, i.e. such that/pattern
    // Arg: "itr" should point to token before clause
    // Return: iterator pointing to the last token in clause
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseConstraintClause(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr, const std::shared_ptr<ConstraintClauseParser>&);
    void addSelectClause();
    void addSuchThatClauses(const std::vector<clauseInfo>& allClauseInfo);
    void addPatternClauses(const std::vector<patternClauseInfo>& allClauseInfo);
    void addWithClauses(const std::vector<clauseInfo>& allClauseInfo);
    void processArgument(const std::vector<std::shared_ptr<Argument>>& arguments);
    void convertSynonymInArgument(const std::shared_ptr<Argument>& argument);

   public:
    ClauseParser() = default;
    explicit ClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator);
    void parseAllClauses(std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::shared_ptr<SelectClause> getSelectClause();
    std::vector<std::shared_ptr<Clause>> getConstraintClauses();
    void setSynonyms(const std::vector<std::shared_ptr<Synonym>>& synonyms);
};
