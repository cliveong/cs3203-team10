#include "QueryTokenizer.h"

#include <utility>

// Constructor
QueryTokenizer::QueryTokenizer(std::string queryString) : queryString(std::move(queryString)) {}

std::vector<std::shared_ptr<QueryToken>> QueryTokenizer::tokenize() {
    std::string::const_iterator itr = queryString.begin();
    std::string::const_iterator itrEnd = queryString.end();

    while (itr != itrEnd) {
        if (isalpha(*itr)) {
            itr = addNameToken(itr, itrEnd);
        } else if (isdigit(*itr)) {
            itr = addIntegerToken(itr, itrEnd);
        } else if (specialChars.find(*itr) != specialChars.end()) {
            std::string tokenString;
            addToken(SPECIAL_CHAR, tokenString += *itr);
            itr++;
        } else if (isspace(*itr)) {
            if ((itr + 1) != itrEnd) {
                addSpaceToken(itr);
            }
            itr++;
        } else {
            std::string tokenString;
            addToken(UNKNOWN, tokenString += *itr);
            itr++;
        }
    }

    addToken(END_OF_QUERY, "EndOfQuery");

    return this->tokens;
}

std::string::const_iterator QueryTokenizer::addNameToken(std::string::const_iterator itr, std::string::const_iterator itrEnd) {
    std::string currentTokenString;
    while (itr != itrEnd && isalnum(*itr)) {
        currentTokenString.push_back(*itr);
        itr++;
    }
    addToken(NAME, currentTokenString);
    return itr;
}

std::string::const_iterator QueryTokenizer::addIntegerToken(std::string::const_iterator itr, std::string::const_iterator itrEnd) {
    std::string currentTokenString;
    while (itr != itrEnd && isdigit(*itr)) {
        currentTokenString.push_back(*itr);
        itr++;
    }
    addToken(INTEGER, currentTokenString);
    return itr;
}

void QueryTokenizer::addSpaceToken(std::string::const_iterator itr) {
    if (*(itr + 1) == STAR) {
        addToken(SPACE, " ");
    } else {
        return;
    }
}

void QueryTokenizer::addToken(TokenType tokenType, const std::string& tokenString) {
    this->tokens.emplace_back(std::make_shared<QueryToken>(tokenType, tokenString));
}