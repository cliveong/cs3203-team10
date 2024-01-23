#pragma once

#include <vector>

#include "../Tokenizer/QueryToken.h"
#include "qps/Clause/SelectClause.h"
#include "qps/Validator/SyntaxValidator.h"

struct elemInfo {
    std::string synonymName;
    std::string attrName;
};

class SelectClauseParser {
   private:
    std::shared_ptr<SelectClause> selectClause;
    std::shared_ptr<SyntaxValidator> syntaxValidator;
    std::vector<elemInfo> allElemInfo;
    bool returnBool;

    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseResultCl(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseTuple(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseElem(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);

   public:
    SelectClauseParser() = default;
    explicit SelectClauseParser(std::shared_ptr<SyntaxValidator> syntaxValidator);
    std::vector<elemInfo> getAllElemInfo();

    // Parses select clause
    // Arg: "itr" should point to the first token in select clause
    // Return: iterator pointing to the last token in select clause
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parse(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);

    bool isReturnBool();
};
