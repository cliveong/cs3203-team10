#include "catch.hpp"
#include "qps/Tokenizer/QueryTokenizer.h"

using namespace std;

bool pred(const shared_ptr<QueryToken>& queryToken1, const shared_ptr<QueryToken>& queryToken2) {
    return (*queryToken1) == (*queryToken2);
}

TEST_CASE("Single Clause Test") {
    std::string queryString = "variable v;\nSelect v";

    QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenResults = queryTokenizer.tokenize();
    std::vector<std::shared_ptr<QueryToken>> expectedTokens = {
        std::make_shared<QueryToken>(NAME, "variable"),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "Select"),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(END_OF_QUERY, "EndOfQuery")};

    REQUIRE(std::equal(tokenResults.begin(), tokenResults.end(), expectedTokens.begin(), expectedTokens.end(), pred));
}

TEST_CASE("Two Clauses Test") {
    std::string queryString = "stmt s;\nSelect s such that Follows* (6, s)";

    QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenResults = queryTokenizer.tokenize();
    std::vector<std::shared_ptr<QueryToken>> expectedTokens = {
        std::make_shared<QueryToken>(NAME, "stmt"),
        std::make_shared<QueryToken>(NAME, "s"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "Select"),
        std::make_shared<QueryToken>(NAME, "s"),
        std::make_shared<QueryToken>(NAME, "such"),
        std::make_shared<QueryToken>(NAME, "that"),
        std::make_shared<QueryToken>(NAME, "Follows"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "*"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(INTEGER, "6"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(NAME, "s"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(END_OF_QUERY, "EndOfQuery")};

    REQUIRE(std::equal(tokenResults.begin(), tokenResults.end(), expectedTokens.begin(), expectedTokens.end(), pred));
}

TEST_CASE("Transitive Clauses Test") {
    std::string queryString = "stmt s;\nSelect s such that Follows *(6, s)";

    QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenResults = queryTokenizer.tokenize();
    std::vector<std::shared_ptr<QueryToken>> expectedTokens = {
        std::make_shared<QueryToken>(NAME, "stmt"),
        std::make_shared<QueryToken>(NAME, "s"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "Select"),
        std::make_shared<QueryToken>(NAME, "s"),
        std::make_shared<QueryToken>(NAME, "such"),
        std::make_shared<QueryToken>(NAME, "that"),
        std::make_shared<QueryToken>(NAME, "Follows"),
        std::make_shared<QueryToken>(SPACE, " "),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "*"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(INTEGER, "6"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(NAME, "s"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(END_OF_QUERY, "EndOfQuery")};

    REQUIRE(std::equal(tokenResults.begin(), tokenResults.end(), expectedTokens.begin(), expectedTokens.end(), pred));
}

TEST_CASE("Two Clauses Test1") {
    std::string queryString = "assign a; variable v;\nSelect a such that Uses(a, v) pattern a(v, _\"4\"_)";

    QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenResults = queryTokenizer.tokenize();
    std::vector<std::shared_ptr<QueryToken>> expectedTokens = {
        std::make_shared<QueryToken>(NAME, "assign"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "variable"),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "Select"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(NAME, "such"),
        std::make_shared<QueryToken>(NAME, "that"),
        std::make_shared<QueryToken>(NAME, "Uses"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(NAME, "pattern"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "_"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "\""),
        std::make_shared<QueryToken>(INTEGER, "4"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "\""),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "_"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(END_OF_QUERY, "EndOfQuery")};

    REQUIRE(std::equal(tokenResults.begin(), tokenResults.end(), expectedTokens.begin(), expectedTokens.end(), pred));
}

TEST_CASE("Wildcard Query Test") {
    std::string queryString = "assign a; variable v;\nSelect a such that Uses (a, v) pattern a(v, _)";

    QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenResults = queryTokenizer.tokenize();
    std::vector<std::shared_ptr<QueryToken>> expectedTokens = {
        std::make_shared<QueryToken>(NAME, "assign"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "variable"),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "Select"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(NAME, "such"),
        std::make_shared<QueryToken>(NAME, "that"),
        std::make_shared<QueryToken>(NAME, "Uses"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(NAME, "pattern"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "_"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(END_OF_QUERY, "EndOfQuery")};

    REQUIRE(std::equal(tokenResults.begin(), tokenResults.end(), expectedTokens.begin(), expectedTokens.end(), pred));
}

TEST_CASE("Invalid Characters Query Test") {
    std::string queryString = "assign a/0; variable?v;\nSelect a\t such that Modifies (a, v)";

    QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
    std::vector<std::shared_ptr<QueryToken>> tokenResults = queryTokenizer.tokenize();
    std::vector<std::shared_ptr<QueryToken>> expectedTokens = {
        std::make_shared<QueryToken>(NAME, "assign"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(UNKNOWN, "/"),
        std::make_shared<QueryToken>(INTEGER, "0"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "variable"),
        std::make_shared<QueryToken>(UNKNOWN, "?"),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ";"),
        std::make_shared<QueryToken>(NAME, "Select"),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(NAME, "such"),
        std::make_shared<QueryToken>(NAME, "that"),
        std::make_shared<QueryToken>(NAME, "Modifies"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, "("),
        std::make_shared<QueryToken>(NAME, "a"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ","),
        std::make_shared<QueryToken>(NAME, "v"),
        std::make_shared<QueryToken>(SPECIAL_CHAR, ")"),
        std::make_shared<QueryToken>(END_OF_QUERY, "EndOfQuery")};

    REQUIRE(std::equal(tokenResults.begin(), tokenResults.end(), expectedTokens.begin(), expectedTokens.end(), pred));
}
