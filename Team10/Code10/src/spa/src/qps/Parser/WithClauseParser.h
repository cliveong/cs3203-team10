#pragma once

#include "ConstraintClauseParser.h"
#include "qps/Validator/SyntaxValidator.h"
#include "qps/WithArgument.h"

class WithClauseParser : public ConstraintClauseParser {
   private:
    std::vector<struct clauseInfo> allClauseInfo;
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseAttrCond(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseAttrCompare(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::shared_ptr<Attribute> parseAttrName(const std::string& attrName);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseArg(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) override;

   public:
    WithClauseParser();
    explicit WithClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseClause(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) override;
    std::vector<struct clauseInfo> getClauseInfo();
};
