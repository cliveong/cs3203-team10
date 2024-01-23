#include "QueryParser.h"

#include "qps/Synonym.h"

QueryParser::QueryParser(const std::vector<std::shared_ptr<QueryToken>>& tokens) : tokens(tokens) {
    declarationParser = std::make_shared<DeclarationParser>(std::make_shared<SyntaxValidator>(this->syntaxValidator));
    clauseParser = std::make_shared<ClauseParser>(std::make_shared<SyntaxValidator>(this->syntaxValidator));
}

std::shared_ptr<Query> QueryParser::parse() {
    auto itr = (*declarationParser).parseAllDeclarations(tokens.begin());
    auto synonyms = (*declarationParser).getSynonyms();

    (*clauseParser).setSynonyms(synonyms);
    (*clauseParser).parseAllClauses(itr);

    return std::make_shared<Query>(synonyms, (*clauseParser).getSelectClause(),
                                   (*clauseParser).getConstraintClauses());
}
