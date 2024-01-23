#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include "ClauseParser.h"
#include "DeclarationParser.h"
#include "qps/Clause/Clause.h"
#include "qps/Clause/SelectClause.h"
#include "qps/Query.h"
#include "qps/Tokenizer/QueryToken.h"
#include "qps/Validator/SyntaxValidator.h"

class QueryParser {
   private:
    std::vector<std::shared_ptr<QueryToken>> tokens;
    std::shared_ptr<DeclarationParser> declarationParser;
    std::shared_ptr<ClauseParser> clauseParser;
    SyntaxValidator syntaxValidator;

   public:
    explicit QueryParser(const std::vector<std::shared_ptr<QueryToken>>& tokens);
    std::shared_ptr<Query> parse();
};
