#include "Tokenizer.h"

#include <iostream>
#include <string>
#include <vector>

Tokenizer::Tokenizer(const std::string& input) : input(input), position(0) {}

std::vector<Token> Tokenizer::tokenize() {
    while (position < input.size()) {
        char currChar = input[position];
        if (isspace(currChar)) {
            position++;
            continue;
        } else if (isalpha(currChar)) {
            tokens.push_back(tokenizeName());
        } else if (isdigit(currChar)) {
            tokens.push_back(tokenizeIntegers());
        } else {
            tokenizeOtherChars(currChar);
        }
    }

    tokens.push_back(Token(TokenTypes::EOF_TOKEN, ""));
    return tokens;
}

void Tokenizer::tokenizeOtherChars(char currChar) {
    std::string tokenValue(1, currChar);

    if (position + 1 < input.size()) {
        tokenValue += input[position + 1];
    }

    if (tokenLookupTable.find(tokenValue) != tokenLookupTable.end()) {
        tokens.push_back(Token(tokenLookupTable[tokenValue], tokenValue));
        if (tokenValue.size() > 1) {
            position++;
        }
    } else if (tokenLookupTable.find(std::string(1, currChar)) !=
               tokenLookupTable.end()) {
        tokens.push_back(Token(tokenLookupTable[std::string(1, currChar)],
                               std::string(1, currChar)));
    } else {
        throw std::runtime_error("Invalid symbol: " + std::string(1, currChar));
    }

    position++;
}

Token Tokenizer::tokenizeName() {
    std::string name;
    while (position < input.size() && (isalnum(input[position]))) {
        name += input[position];
        position++;
    }

    auto it = keyWordLookupTable.find(name);
    if (it != keyWordLookupTable.end()) {
        return Token(it->second, name);
    } else {
        return Token(TokenTypes::NAME, name);
    }
}

Token Tokenizer::tokenizeIntegers() {
    std::string integer;
    while (position < input.size() && isdigit(input[position])) {
        integer += input[position];
        position++;
    }

    if (integer.size() > 1 && integer[0] == '0') {
        throw std::runtime_error("Error: No trailing zeroes allowed in " + integer);
    }

    return Token(TokenTypes::INTEGER, integer);
}
