#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "qps/AttrRef.h"
#include "qps/Clause/PatternClause/AssignPatternClause.h"
#include "qps/Clause/WithClause.h"
#include "qps/Query.h"
#include "qps/WithArgument.h"

class SemanticValidator {
   private:
    void validateDeclarations(const std::vector<std::shared_ptr<Synonym>>& synonyms);
    void validateSelectClause(const std::shared_ptr<SelectClause>& selectClause);
    void validateSynonymDeclared(const std::shared_ptr<Synonym>& synonym);
    // Checks that synonym in arguments of clause
    void validateSynonymInArgDeclared(const std::shared_ptr<Clause>& clause);

    void validateConstraintClause(const std::shared_ptr<Clause>& clause);

    bool isProcedure(const std::shared_ptr<Synonym>& synonym);
    bool isStatement(const std::shared_ptr<Synonym>& synonym);
    bool isVariable(const std::shared_ptr<Synonym>& synonym);
    bool isWildcard(const std::shared_ptr<Argument>& argument);

    void validateStmtStmtClause(const std::shared_ptr<Clause>& clause);
    void validateProcProcClause(const std::shared_ptr<Clause>& clause);
    void validateUsesModifiesClause(const std::shared_ptr<Clause>& clause);
    void validatePattern(const std::shared_ptr<PatternClause>& clause);
    void validateWithClause(const std::shared_ptr<WithClause>& clause);
    void validateAttr(const std::shared_ptr<WithArgument>& withArg);
    void validateAttr(const std::shared_ptr<AttrRef>& attrRef);

   public:
    void validateSemantic(const std::shared_ptr<Query>& query);
};
