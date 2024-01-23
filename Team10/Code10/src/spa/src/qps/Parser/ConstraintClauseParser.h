#pragma once

#include <memory>
#include <vector>

#include "qps/Argument.h"
#include "qps/Tokenizer/QueryToken.h"
#include "qps/Validator/SyntaxValidator.h"

struct clauseInfo {  // stores clause information
    bool isNot;
    std::string clauseType;
    std::vector<std::shared_ptr<Argument>> arguments;
};

// Super class of clause parsers that parse two arguments
class ConstraintClauseParser {
   private:
    bool isArgTerminatingString(const std::string& endStr);

   protected:
    const std::string OPEN_PARENTHESIS = "(";
    const std::string COMMA = ",";
    const std::string CLOSE_PARENTHESIS = ")";
    const std::string INVERTED_COMMA = "\"";
    const std::string WILDCARD = "_";
    const std::string SPACE = " ";
    const std::string STAR = "*";

    std::shared_ptr<SyntaxValidator> syntaxValidator;
    // contains info of current clause being parsed
    clauseInfo currentClauseInfo;

    // Parses the argument starting from the token pointed to by itr until  ',' OR ')' is found or
    // if tokens have reached token pointed to by itrEnd
    virtual std::vector<std::shared_ptr<QueryToken>>::const_iterator parseArg(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseNot(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);

   public:
    ConstraintClauseParser() = default;
    explicit ConstraintClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator);
    virtual std::vector<std::shared_ptr<QueryToken>>::const_iterator parseClause(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) = 0;
    ArgType parseArgType(const std::string& argStr);
};
