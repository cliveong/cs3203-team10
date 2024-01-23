#include "QueryToken.h"
#include "qps/Validator/ValidatorErrors/SyntaxException.h"

#include <utility>

QueryToken::QueryToken(TokenType tokenType, std::string tokenString) : tokenType(std::move(tokenType)), tokenString(std::move(tokenString)) {}

bool QueryToken::operator==(const QueryToken& token) {
    return this->tokenString == token.tokenString;
}

std::string QueryToken::getTokenString() {
    if (this == nullptr) {
        throw std::exception();
    }
    return this->tokenString;
}

TokenType QueryToken::getTokenType() {
    if (this == nullptr) {
        throw std::exception();
    }
    return this->tokenType;
}
