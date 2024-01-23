#pragma once

#include <map>
#include <string>
#include <vector>

enum class TokenTypes {
    NAME,
    READ,
    PRINT,
    IF,
    THEN,
    ELSE,
    WHILE,
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    ASSIGN,
    NOT,
    AND,
    OR,
    REL_OPERATORS,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    SEMICOLON,
    PROCEDURE,
    EOF_TOKEN,
    CALL,
};

struct Token {
    TokenTypes type;
    std::string value;

    Token(TokenTypes t, const std::string& v) : type(t), value(v) {}
};

class Tokenizer {
   public:
    Tokenizer(const std::string& input);
    std::vector<Token> tokens;

    std::vector<Token> tokenize();

   private:
    std::string input;
    size_t position;
    std::map<std::string, TokenTypes> keyWordLookupTable = {
        {"procedure", TokenTypes::PROCEDURE},
        {"read", TokenTypes::READ},
        {"print", TokenTypes::PRINT},
        {"if", TokenTypes::IF},
        {"while", TokenTypes::WHILE},
        {"then", TokenTypes::THEN},
        {"else", TokenTypes::ELSE},
        {"call", TokenTypes::CALL}};
    std::map<std::string, TokenTypes> tokenLookupTable = {
        {"+", TokenTypes::PLUS},
        {"-", TokenTypes::MINUS},
        {"*", TokenTypes::MULTIPLY},
        {"/", TokenTypes::DIVIDE},
        {"%", TokenTypes::MODULO},
        {"(", TokenTypes::LEFT_PAREN},
        {")", TokenTypes::RIGHT_PAREN},
        {"{", TokenTypes::LEFT_BRACE},
        {"}", TokenTypes::RIGHT_BRACE},
        {";", TokenTypes::SEMICOLON},
        {"!", TokenTypes::NOT},
        {"!=", TokenTypes::REL_OPERATORS},
        {"==", TokenTypes::REL_OPERATORS},
        {">=", TokenTypes::REL_OPERATORS},
        {"<=", TokenTypes::REL_OPERATORS},
        {">", TokenTypes::REL_OPERATORS},
        {"<", TokenTypes::REL_OPERATORS},
        {"&&", TokenTypes::AND},
        {"||", TokenTypes::OR},
        {"=", TokenTypes::ASSIGN}};

    Token tokenizeName();
    Token tokenizeIntegers();
    void tokenizeOtherChars(char currChar);
};
