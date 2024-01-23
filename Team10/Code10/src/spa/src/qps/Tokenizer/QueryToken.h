#pragma once

#include <string>

enum TokenType {
    NAME,
    INTEGER,
    SPECIAL_CHAR,
    SPACE,
    UNKNOWN,
    END_OF_QUERY
};

class QueryToken {
   private:
    TokenType tokenType;
    std::string tokenString;

   public:
    explicit QueryToken(TokenType tokenType, std::string tokenString);
    TokenType getTokenType();
    std::string getTokenString();
    bool operator==(const QueryToken& token);
};
