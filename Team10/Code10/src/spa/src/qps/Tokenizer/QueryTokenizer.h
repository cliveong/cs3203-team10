#pragma once

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "QueryToken.h"

class QueryTokenizer {
   private:
    std::string queryString;
    std::vector<std::shared_ptr<QueryToken>> tokens;
    const char STAR = '*';
    std::unordered_set<char> specialChars = {'*', ',', '(', ')', '\"', '_', ';', '+', '-', '/', '%', '=', '#', '<', '>', '.'};
    void addToken(TokenType tokenType, const std::string& tokenString);
    std::string::const_iterator addNameToken(std::string::const_iterator itr, std::string::const_iterator itrEnd);
    std::string::const_iterator addIntegerToken(std::string::const_iterator itr, std::string::const_iterator itrEnd);
    void addSpaceToken(std::string::const_iterator itr);

   public:
    explicit QueryTokenizer(std::string queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenize();
};