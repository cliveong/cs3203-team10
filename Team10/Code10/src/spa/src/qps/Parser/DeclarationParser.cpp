#include "DeclarationParser.h"

#include <utility>

DeclarationParser::DeclarationParser(std::shared_ptr<SyntaxValidator> validator) : validator(std::move(validator)) {}

std::vector<std::shared_ptr<QueryToken>>::const_iterator DeclarationParser::parseAllDeclarations(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr) {
    std::string designEntityString = (*itr)->getTokenString();

    //  first token must be a design entity for declaration, if not, stop declaration parsing
    if (designEntitiesMap.find(designEntityString) == designEntitiesMap.end()) {
        return itr;
    }

    std::vector<std::shared_ptr<QueryToken>>::const_iterator lastDeclarationToken;

    while ((*itr)->getTokenType() != END_OF_QUERY) {
        if (designEntitiesMap.find(designEntityString) != designEntitiesMap.end()) {
            lastDeclarationToken = parseDeclaration(++itr, designEntitiesMap[designEntityString]);
            itr = lastDeclarationToken + 1;
            designEntityString = (*itr)->getTokenString();
        } else {
            break;
        }
    }
    return ++lastDeclarationToken;
}

std::vector<std::shared_ptr<QueryToken>>::const_iterator DeclarationParser::parseDeclaration(
    std::vector<std::shared_ptr<QueryToken>>::const_iterator itr, const SynonymType& synonymType) {
    validator->validateDeclaration(itr);

    std::string synonymName = (*itr)->getTokenString();
    synonyms.emplace_back(std::make_shared<Synonym>(synonymType, synonymName));

    std::string nextTokenStr = (*(itr + 1))->getTokenString();
    while (nextTokenStr == COMMA) {
        itr = itr + 2;
        synonymName = (*itr)->getTokenString();
        synonyms.emplace_back(std::make_shared<Synonym>(synonymType, synonymName));
        nextTokenStr = (*(itr + 1))->getTokenString();
    }

    return ++itr;
}

std::vector<std::shared_ptr<Synonym>> DeclarationParser::getSynonyms() {
    return this->synonyms;
}
