#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../Synonym.h"
#include "../Tokenizer/QueryToken.h"
#include "qps/Validator/SyntaxValidator.h"

class DeclarationParser {
   private:
    const std::string SEMICOLON = ";";
    const std::string COMMA = ",";
    std::shared_ptr<SyntaxValidator> validator;
    std::unordered_map<std::string, SynonymType> designEntitiesMap = {
        {"stmt", STMT},
        {"read", READ},
        {"print", PRINT},
        {"call", CALL},
        {"while", WHILE},
        {"if", IF},
        {"assign", ASSIGN},
        {"variable", VARIABLE},
        {"constant", CONSTANT},
        {"procedure", PROCEDURE}};
    std::vector<std::shared_ptr<Synonym>> synonyms;

    // Parses the declaration tokens by starting from the current token until ";"
    // Arg: "itr" should point at the token containing the design entity string
    // Returns: the iterator pointing to the ";" token
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseDeclaration(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr, const SynonymType& synonymType);

   public:
    DeclarationParser() = default;
    explicit DeclarationParser(std::shared_ptr<SyntaxValidator> validator);

    // Return: the iterator pointing to the next token after parsing of last declaration
    std::vector<std::shared_ptr<QueryToken>>::const_iterator parseAllDeclarations(
        std::vector<std::shared_ptr<QueryToken>>::const_iterator itr);
    std::vector<std::shared_ptr<Synonym>> getSynonyms();
};
