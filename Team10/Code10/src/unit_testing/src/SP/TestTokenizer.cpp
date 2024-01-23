#include "catch.hpp"
#include "sp/Tokenizer.h"

TEST_CASE("Tokenizer Test Cases") {
    SECTION("Tokenize Names and Keywords") {
        Tokenizer tokenizer("x if while procedure print read then else num2");
        std::vector<Token> tokens = tokenizer.tokenize();

        REQUIRE(tokens.size() == 10);

        REQUIRE(tokens[0].type == TokenTypes::NAME);
        REQUIRE(tokens[0].value == "x");

        REQUIRE(tokens[1].type == TokenTypes::IF);

        REQUIRE(tokens[2].type == TokenTypes::WHILE);

        REQUIRE(tokens[3].type == TokenTypes::PROCEDURE);

        REQUIRE(tokens[4].type == TokenTypes::PRINT);

        REQUIRE(tokens[5].type == TokenTypes::READ);

        REQUIRE(tokens[6].type == TokenTypes::THEN);
        REQUIRE(tokens[7].type == TokenTypes::ELSE);

        REQUIRE(tokens[8].type == TokenTypes::NAME);
        REQUIRE(tokens[8].value == "num2");
    }

    SECTION("Tokenize Integers") {
        Tokenizer tokenizer("123 456 0 -789");

        std::vector<Token> tokens = tokenizer.tokenize();

        REQUIRE(tokens.size() == 6);

        REQUIRE(tokens[0].type == TokenTypes::INTEGER);
        REQUIRE(tokens[0].value == "123");

        REQUIRE(tokens[1].type == TokenTypes::INTEGER);
        REQUIRE(tokens[1].value == "456");

        REQUIRE(tokens[2].type == TokenTypes::INTEGER);
        REQUIRE(tokens[2].value == "0");

        REQUIRE(tokens[3].type == TokenTypes::MINUS);
        REQUIRE(tokens[3].value == "-");
    }

    SECTION("Tokenize Operators") {
        Tokenizer tokenizer("+ - * / % = > < >= <= == ! && || !=");

        std::vector<Token> tokens = tokenizer.tokenize();

        REQUIRE(tokens.size() == 16);

        REQUIRE(tokens[0].type == TokenTypes::PLUS);
        REQUIRE(tokens[0].value == "+");

        REQUIRE(tokens[1].type == TokenTypes::MINUS);
        REQUIRE(tokens[1].value == "-");

        REQUIRE(tokens[2].type == TokenTypes::MULTIPLY);
        REQUIRE(tokens[2].value == "*");

        REQUIRE(tokens[3].type == TokenTypes::DIVIDE);
        REQUIRE(tokens[3].value == "/");

        REQUIRE(tokens[4].type == TokenTypes::MODULO);
        REQUIRE(tokens[4].value == "%");

        REQUIRE(tokens[5].type == TokenTypes::ASSIGN);
        REQUIRE(tokens[5].value == "=");

        REQUIRE(tokens[6].type == TokenTypes::REL_OPERATORS);
        REQUIRE(tokens[6].value == ">");

        REQUIRE(tokens[7].type == TokenTypes::REL_OPERATORS);
        REQUIRE(tokens[7].value == "<");

        REQUIRE(tokens[8].type == TokenTypes::REL_OPERATORS);
        REQUIRE(tokens[8].value == ">=");

        REQUIRE(tokens[9].type == TokenTypes::REL_OPERATORS);
        REQUIRE(tokens[9].value == "<=");

        REQUIRE(tokens[10].type == TokenTypes::REL_OPERATORS);
        REQUIRE(tokens[10].value == "==");

        REQUIRE(tokens[11].type == TokenTypes::NOT);
        REQUIRE(tokens[11].value == "!");

        REQUIRE(tokens[12].type == TokenTypes::AND);
        REQUIRE(tokens[12].value == "&&");

        REQUIRE(tokens[13].type == TokenTypes::OR);
        REQUIRE(tokens[13].value == "||");

        REQUIRE(tokens[14].type == TokenTypes::REL_OPERATORS);
        REQUIRE(tokens[14].value == "!=");
    }

    SECTION("Tokenize Parentheses, Braces, and Semicolon") {
        Tokenizer tokenizer("(){};");

        std::vector<Token> tokens = tokenizer.tokenize();

        REQUIRE(tokens.size() == 6);

        REQUIRE(tokens[0].type == TokenTypes::LEFT_PAREN);
        REQUIRE(tokens[0].value == "(");

        REQUIRE(tokens[1].type == TokenTypes::RIGHT_PAREN);
        REQUIRE(tokens[1].value == ")");

        REQUIRE(tokens[2].type == TokenTypes::LEFT_BRACE);
        REQUIRE(tokens[2].value == "{");

        REQUIRE(tokens[3].type == TokenTypes::RIGHT_BRACE);
        REQUIRE(tokens[3].value == "}");

        REQUIRE(tokens[4].type == TokenTypes::SEMICOLON);
        REQUIRE(tokens[4].value == ";");
    }

    SECTION("Tokenize Invalid Syntax/Symbols") {
        Tokenizer tokenizer("$");
        REQUIRE_THROWS_WITH(tokenizer.tokenize(), "Invalid symbol: $");

        Tokenizer tokenizer2("&");
        REQUIRE_THROWS(tokenizer2.tokenize());

        Tokenizer tokenizer3("0001");
        REQUIRE_THROWS_WITH(tokenizer3.tokenize(), "Error: No trailing zeroes allowed in 0001");
    }

    SECTION("Tokenize Mixed Input") {
        Tokenizer tokenizer("x = 42 + y * (5 - 2);");

        std::vector<Token> tokens = tokenizer.tokenize();

        REQUIRE(tokens.size() == 13);

        REQUIRE(tokens[0].type == TokenTypes::NAME);
        REQUIRE(tokens[0].value == "x");

        REQUIRE(tokens[1].type == TokenTypes::ASSIGN);

        REQUIRE(tokens[2].type == TokenTypes::INTEGER);
        REQUIRE(tokens[2].value == "42");

        REQUIRE(tokens[3].type == TokenTypes::PLUS);

        REQUIRE(tokens[4].type == TokenTypes::NAME);
        REQUIRE(tokens[4].value == "y");

        REQUIRE(tokens[5].type == TokenTypes::MULTIPLY);

        REQUIRE(tokens[6].type == TokenTypes::LEFT_PAREN);
        REQUIRE(tokens[6].value == "(");

        REQUIRE(tokens[7].type == TokenTypes::INTEGER);
        REQUIRE(tokens[7].value == "5");

        REQUIRE(tokens[8].type == TokenTypes::MINUS);

        REQUIRE(tokens[9].type == TokenTypes::INTEGER);
        REQUIRE(tokens[9].value == "2");

        REQUIRE(tokens[10].type == TokenTypes::RIGHT_PAREN);
        REQUIRE(tokens[10].value == ")");

        REQUIRE(tokens[11].type == TokenTypes::SEMICOLON);

        REQUIRE(tokens[12].type == TokenTypes::EOF_TOKEN);
    }
}
