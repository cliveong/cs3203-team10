#include <string>

#include "catch.hpp"
#include "qps/Clause/PatternClauseFactory.h"
#include "qps/Clause/SuchThatClauseFactory.h"
#include "qps/Clause/WithClauseFactory.h"
#include "qps/Parser/QueryParser.h"
#include "qps/Tokenizer/QueryTokenizer.h"
#include "qps/Validator/SemanticValidator.h"
#include "qps/Validator/ValidatorErrors/SyntaxException.h"
#include "sp/Parsers/ExprParser.h"
#include "sp/Parsers/ParserContext.h"
#include "sp/Tokenizer.h"

using namespace std;

std::shared_ptr<ExprNode> buildAST(std::string pattern) {
    Tokenizer tokenizer(pattern);
    std::vector<Token> tokens = tokenizer.tokenize();
    std::shared_ptr<ParserContext> parserContext = std::make_shared<ParserContext>(tokens);
    ExprParser exprParser(parserContext);
    return exprParser.parse();
}

TEST_CASE("Declaration Parse Test") {
    SECTION("Invalid Declaration Type Test") {
        std::string queryString = "assignment a; variable v, v1;\nSelect a";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Synonym Test") {
        std::string queryString = "assign _a; variable v, v1;\nSelect a";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Semicolon Test") {
        std::string queryString = "assign a;; variable v, v1;\nSelect a";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Character Test") {
        std::string queryString = "assign a; \"\" variable v, v1;\nSelect a";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("No declaration Test") {
        std::string queryString = "     \nSelect a";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms;
        std::vector<std::shared_ptr<Elem>> selectedElements = {std::make_shared<Synonym>(UNDECLARED, "a")};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }
}

TEST_CASE("Select Clause Parse Test") {
    SECTION("Select One Synonym Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect a";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Select One AttrRef Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect a.stmt#";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {std::make_shared<AttrRef>(synonyms[0], std::make_shared<Attribute>(STMT_ATTR))};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        auto boolean = (*actualQuery).equals(expectedQuery);
        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Positive BOOLEAN Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect BOOLEAN";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements;
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, true);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Positive BOOLEAN w/o declaration Test") {
        std::string queryString = "  \nSelect BOOLEAN";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms;

        std::vector<std::shared_ptr<Elem>> selectedElements;
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, true);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Positive Tuple Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect <a,v>";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 2};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Positive Tuple With AttrRef At the Back Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect <a,v.varName>";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms[0],
                                                               std::make_shared<AttrRef>(synonyms[1], std::make_shared<Attribute>(VARNAME_ATTR))};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Positive Tuple With Multiple AttrRef At the Front Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect <a.stmt#,v.varName, v1>";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {std::make_shared<AttrRef>(synonyms[0], std::make_shared<Attribute>(STMT_ATTR)),
                                                               std::make_shared<AttrRef>(synonyms[1], std::make_shared<Attribute>(VARNAME_ATTR)),
                                                               synonyms[2]};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Select Single BOOLEAN Synonym Test") {
        std::string queryString = "stmt BOOLEAN; variable v, v1;\nSelect BOOLEAN";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(STMT, "BOOLEAN"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Select Tuple BOOLEAN Synonym Test") {
        std::string queryString = "stmt BOOLEAN; variable v, v1;\nSelect <BOOLEAN, v, v1>";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(STMT, "BOOLEAN"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(VARIABLE, "v1")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses;
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Synonym Wrong Syntax Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect _a1";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Select Clause Wrong Syntax Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect ** a1";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Select Clause Missing Select Test") {
        std::string queryString = "assign a; variable v, v1;\n";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Select Clause Missing Synonym Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Select Clause Missing Attribute Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect a.";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Select Clause Attribute Without Synonym Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect .stmt#";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Tuple Return Missing > Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect <a,v ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Tuple Return Missing < Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect a,v> ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Tuple Return Missing Comma Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect a v v1> ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("AttrRef In Tuple Missing Attribute Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect <a. , v, v1> ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("AttrRef In Tuple Missing Synonym Test") {
        std::string queryString = "assign a; variable v, v1;\nSelect <a, .varName, v1> ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("Modifies Clause Parse Test") {
    SECTION("First Arg Assign Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(a, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Modifies",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Read Test") {
        std::string queryString = "read re; variable v;\nSelect v such that Modifies(re, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(READ, "re"),
            std::make_shared<Synonym>(VARIABLE, "v")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Modifies",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Integer Test") {
        std::string queryString = "variable v;\nSelect v such that Modifies(10, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Modifies",
                                 std::make_shared<Argument>(INT_ARG, "10"),
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Second Arg String Test") {
        std::string queryString = "variable v;\nSelect v such that Modifies(v, \"x\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Modifies",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(STRING_ARG, "x"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Single Not Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that not Modifies(a, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};

        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};
        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Modifies",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]), true)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Second Arg Invalid Integer Test") {
        std::string queryString = "variable v;\nSelect v such that Modifies(v, 3)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Integer Wrong Syntax Test") {
        std::string queryString = "variable v;\nSelect v such that Modifies(01, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Synonym Wrong Syntax Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(_a, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("String Wrong Syntax Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(a, \"x)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Wildcard Wrong Syntax Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(__, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing First Argument Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing that Test") {
        std::string queryString = "stmt s; variable v;\nSelect s such    *";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing relRef Test") {
        std::string queryString = "stmt s; variable v;\nSelect s such that";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Open Parenthesis in Argument Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies((a, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Close Parenthesis in Argument Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(a,) v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Parentheses Pair in Argument Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(a(, v))";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Three Arguments Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(a, v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Open Parenthesis Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies a, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Close Parenthesis Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Modifies(a, v";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Not Wrong Position Test") {
        std::string queryString = "variable v;\nSelect v not such that Modifies(v, 3)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Space In Argument Test") {
        std::string queryString = "variable v;\nSelect v such that Modifies(1 1, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("Uses Clause Parse Test") {
    SECTION("Second Arg String Test") {
        std::string queryString = "variable v;\nSelect v such that Uses (v, \"x\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Uses",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(STRING_ARG, "x"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Second Arg Synonym Test") {
        std::string queryString = "print print; variable v;\nSelect v such that Uses (print, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(PRINT, "print"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Uses",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Single Not Test") {
        std::string queryString = "variable v;\nSelect v such that not Uses (v, \"x\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Uses",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(STRING_ARG, "x"), true)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Second Arg Integer Wrong Syntax Test") {
        std::string queryString = "if if;\nSelect if such that Uses (if, 5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Three Argument Wrong Syntax Test") {
        std::string queryString = "assign a; variable v;\nSelect v such that Uses(a, v, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("Parent Clause Parse Test") {
    SECTION("First Arg Integer Test") {
        std::string queryString = "read read;\nSelect read such that Parent(2, read)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(READ, "read")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Parent",
                                 std::make_shared<Argument>(INT_ARG, "2"),
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Second Arg Integer Test") {
        std::string queryString = "while w; \nSelect w such that Parent*(w, 7)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(WHILE, "w")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("Parent*",
                                 std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                 std::make_shared<Argument>(INT_ARG, "7"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Invalid String Test") {
        std::string queryString = "stmt s;\nSelect s such that Parent*(\"x\", s)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Second Arg Invalid String Test") {
        std::string queryString = "stmt s;\nSelect s such that Parent(s, \"x\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Space Before * Test") {
        std::string queryString = "stmt s;\nSelect s such that Parent *(s, \"x\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("Assign Pattern Clause Parse Test") {
    SECTION("Exact Match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, \"x + y * z - t\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                 std::make_shared<ExprArgument>(EXPRESSION, "\"x+y*z-t\"", buildAST("x+y*z-t")),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Exact Match Bracket Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, \"(x + y) * (z - t)\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                 std::make_shared<ExprArgument>(EXPRESSION, "\"(x+y)*(z-t)\"", buildAST("(x+y)*(z-t)")),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Partial Match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _\"x+a+b*c\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                 std::make_shared<ExprArgument>(EXPRESSION, "_\"x+a+b*c\"_", buildAST("x+a+b*c")),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Partial Match Bracket Test") {
        std::string queryString = "assign a;\nSelect a pattern a (_, _\"c * (a + b) + 5 - b\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<ExprArgument>(EXPRESSION, "_\"c*(a+b)+5-b\"_", buildAST("c*(a+b)+5-b")),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Multiple Bracket Test") {
        std::string queryString = "assign a;\nSelect a pattern a (_, _\"((c * (a + b)) + (5 - b))\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<ExprArgument>(EXPRESSION, "_\"((c*(a+b))+(5-b))\"_", buildAST("((c*(a+b))+(5-b))")),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Wildcard Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                 std::make_shared<ExprArgument>(WILDCARD, "_", make_shared<ExprNode>()),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Double Wildcard Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(_, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<ExprArgument>(WILDCARD, "_", make_shared<ExprNode>()),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Single Not Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern not a (_, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("AssignPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<ExprArgument>(WILDCARD, "_", make_shared<ExprNode>()),
                                 true)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Pattern Synonym Missing Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern (v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Exact Match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, \"x^4\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Exact Bracket Match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, \"(x^4)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Partial Match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _\"1+2\"_*-x\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Partial Bracket Match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _\"1+()*2-x\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Partial Match Switch Wildcard and Inverted Comma Test") {
        std::string queryString = "assign a;\nSelect a pattern a(_, \"_x_\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Empty Expr in Partial Match Test") {
        std::string queryString = "assign a;\nSelect a pattern a(_, _\"\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("First Arg Invalid Integer Test") {
        std::string queryString = "assign a;\nSelect a pattern a(15, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Open Parenthesis Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a((v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Close Parenthesis Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _))";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Parentheses Pair Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a((v, _))";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Comma Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v,, _))";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Open Parenthesis Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Close Parenthesis Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _  ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing pattern in patternCond Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern  ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Only '(' in pattern Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a( ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Only First Arg Present in pattern Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(_, ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing expression in partial match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(_, _\" ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing expression in exact match Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(_, \" ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("First term Operator in Pattern Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(v, _\"+temp\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Not Wrong Position Test") {
        std::string queryString = "assign a; variable v;\nSelect a not pattern a(_, _) ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Space in Pattern Test") {
        std::string queryString = "assign a; variable v;\nSelect <a.stmt#, v> pattern a(v, \"(1 2 * 3)\") ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Space in Pattern1 Test") {
        std::string queryString = "assign a; variable v;\nSelect a pattern a(, _\"(1) * 3)\"_) ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("If Pattern Clause Parse Test") {
    SECTION("First Arg Synonym Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(v, _, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(IF, "if"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("IfPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg String Test") {
        std::string queryString = "if if;\nSelect if pattern if(\"x\", _, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(IF, "if")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("IfPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(STRING_ARG, "x"),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Wildcard Test") {
        std::string queryString = "if if;\nSelect if pattern if(_, _, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(IF, "if")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("IfPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Single Not Test") {
        std::string queryString = "if if;\nSelect if pattern not if(_, _, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(IF, "if")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("IfPattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 true)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Invalid Integer Test") {
        std::string queryString = "if if;\nSelect if pattern if(10, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Second Arg Invalid Synonym Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_, v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Second Arg Invalid Integer Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_, 5, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Third Arg Invalid Synonym Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_, _, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Third Arg Invalid String Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_, _, \"rasd\")";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Third Arg Invalid Integer Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_, _, 7)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Open Parenthesis Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_,( _, 7)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Close Parenthesis Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_,) _, 7)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Parenthesis Pair Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_(, _, 7))";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Comma Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if(_, _,, 7)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Open Parenthesis Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if  _, _, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Close Parenthesis Test") {
        std::string queryString = "if if; variable v;\nSelect if pattern if ( _, _, _    ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Not Wrong Position Test") {
        std::string queryString = "if if; variable v;\nSelect if not pattern if ( _, _, _)    ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("While Pattern Clause Parse Test") {
    SECTION("First Arg Synonym Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(WHILE, "w"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("WhilePattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg String Test") {
        std::string queryString = "while if;\nSelect if pattern if(\"xyz\", _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(WHILE, "if")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("WhilePattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(STRING_ARG, "xyz"),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Wildcard Test") {
        std::string queryString = "while while;\nSelect while pattern while(_, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(WHILE, "while")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("WhilePattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Single Not Test") {
        std::string queryString = "while while;\nSelect while pattern not while(_, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory factory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(WHILE, "while")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            factory.createClause("WhilePattern",
                                 synonyms[0],
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 std::make_shared<Argument>(WILDCARD, "_"),
                                 true)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("First Arg Invalid Integer Test") {
        std::string queryString = "while w;\nSelect w pattern w(10, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Second Arg Invalid Synonym Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(_, v)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Second Arg Invalid Integer Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(_, 5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Open Parenthesis Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(_, (5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Close Parenthesis Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w()_, 5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Parenthesis Pair Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(_,() 5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Extra Comma Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(,_, 5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Open Parenthesis Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w  ,_, 5)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Missing Close Parenthesis Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w(,_, 5    ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Not Wrong Position Test") {
        std::string queryString = "while w; variable v;\nSelect w pattern w not (_, 5)    ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("With Clause Parse Test") {
    SECTION("Procedure procName Test") {
        std::string queryString = "procedure p;\nSelect p  with p.procName = \"main\"";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(PROCEDURE, "p")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(PROCNAME_ATTR)),
                                           make_shared<Argument>(STRING_ARG, "main"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Call procName Test") {
        std::string queryString = "call c;\nSelect c  with \"x\" = c.procName";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(CALL, "c")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<Argument>(STRING_ARG, "x"),
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(PROCNAME_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Call stmt# Test") {
        std::string queryString = "call c;\nSelect c with 55=  c.stmt#";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(CALL, "c")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<Argument>(INT_ARG, "55"),
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(STMT_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Variable varName Test") {
        std::string queryString = "variable v;\nSelect v  with \"var\" = v.varName";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {

            withClauseFactory.createClause("WithClause",
                                           make_shared<Argument>(STRING_ARG, "var"),
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(VARNAME_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Read varName Test") {
        std::string queryString = "read re;\nSelect re with re.varName = \"xyz\"";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(READ, "re")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(VARNAME_ATTR)),
                                           make_shared<Argument>(STRING_ARG, "xyz"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Read stmt# Test") {
        std::string queryString = "read read;\nSelect read with 10=  read.stmt#";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(READ, "read")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<Argument>(INT_ARG, "10"),
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(STMT_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Print varName Test") {
        std::string queryString = "procedure p; print pn;\nSelect pn with p.procName = pn.varName";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(PROCEDURE, "p"),
            std::make_shared<Synonym>(PRINT, "pn")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(PROCNAME_ATTR)),
                                           make_shared<WithArgument>(SYNONYM, synonyms[1], std::make_shared<Attribute>(VARNAME_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Print stmt# Test") {
        std::string queryString = "print pn;\nSelect pn with pn.stmt# =     100";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(PRINT, "pn")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(STMT_ATTR)),
                                           make_shared<Argument>(INT_ARG, "100"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Constant value Test") {
        std::string queryString = "constant c;\nSelect c with c.value = 1";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(CONSTANT, "c")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(PROCNAME_ATTR)),
                                           make_shared<Argument>(INT_ARG, "1"), false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Statement and Assign stmt# Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.stmt# = a.stmt#";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {std::make_shared<Synonym>(STMT, "s"),
                                                          std::make_shared<Synonym>(ASSIGN, "a")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(STMT_ATTR)),
                                           make_shared<WithArgument>(SYNONYM, synonyms[1], std::make_shared<Attribute>(STMT_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("If and While stmt# Test") {
        std::string queryString = "if if; while w;\nSelect if with if.stmt# = w.stmt#";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {std::make_shared<Synonym>(IF, "if"),
                                                          std::make_shared<Synonym>(WHILE, "w")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(STMT_ATTR)),
                                           make_shared<WithArgument>(SYNONYM, synonyms[1], std::make_shared<Attribute>(STMT_ATTR)),
                                           false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Single Not Test") {
        std::string queryString = "procedure p;\nSelect p  with not p.procName = \"main\"";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();

        WithClauseFactory withClauseFactory;
        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(PROCEDURE, "p")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);

        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            withClauseFactory.createClause("WithClause",
                                           make_shared<WithArgument>(SYNONYM, synonyms[0], std::make_shared<Attribute>(PROCNAME_ATTR)),
                                           make_shared<Argument>(STRING_ARG, "main"), true)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Invalid Wildcard Test") {
        std::string queryString = "print pn;\nSelect pn with pn.stmt# =     _";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Synonym Missing '.' Test") {
        std::string queryString = "print pn;\nSelect pn with pn stmt# =     _";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Synonym Missing '=' Test") {
        std::string queryString = "print pn;\nSelect pn with pn stmt#      _";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Synonym Missing first ref Test") {
        std::string queryString = "print pn;\nSelect pn with    ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Synonym Missing second ref Test") {
        std::string queryString = "print pn;\nSelect pn with    pn.stmt# =  ";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Not Wrong Position Test") {
        std::string queryString = "print pn;\nSelect pn  with pn.stmt# not =  5";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}

TEST_CASE("Multi Clause Parse Test") {
    SECTION("Pattern and Modifies Test") {
        std::string queryString = "assign a; variable v;\nSelect a such that Modifies(a, v) pattern a(v, _\"x\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory suchThatFactory;
        PatternClauseFactory patternFactory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            suchThatFactory.createClause("Modifies",
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                         false),
            patternFactory.createClause("AssignPattern",
                                        synonyms[0],
                                        std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                        std::make_shared<ExprArgument>(EXPRESSION, "_\"x\"_", buildAST("x")),
                                        false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Multiple Such That And Clause Test") {
        std::string queryString =
            "assign a; variable v; while w;\nSelect a such that Modifies(a, v) and not Parent*(w, a) and Follows*(1, a)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory suchThatFactory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(WHILE, "w")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            suchThatFactory.createClause("Modifies",
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                         false),
            suchThatFactory.createClause("Parent*",
                                         std::make_shared<Argument>(SYNONYM, synonyms[2]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         true),
            suchThatFactory.createClause("Follows*",
                                         std::make_shared<Argument>(INT_ARG, "1"),
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Multiple Such That Not Without And Clause Test") {
        std::string queryString =
            "assign a; variable v; while w;\nSelect a such that Modifies(a, v) such that not Parent*(w, a) such that Follows*(1, a)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory suchThatFactory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(WHILE, "w")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            suchThatFactory.createClause("Modifies",
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                         false),
            suchThatFactory.createClause("Parent*",
                                         std::make_shared<Argument>(SYNONYM, synonyms[2]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         true),
            suchThatFactory.createClause("Follows*",
                                         std::make_shared<Argument>(INT_ARG, "1"),
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Multiple Pattern And Clause Test") {
        std::string queryString =
            "assign a;\nSelect a pattern a(_, _\"v\"_) and a(_, _\"x\"_) and a(_, _\"y\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        PatternClauseFactory patternFactory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
        };
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.end() - 1, synonyms.end()};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            patternFactory.createClause("AssignPattern",
                                        synonyms[0],
                                        std::make_shared<Argument>(WILDCARD, "_"),
                                        std::make_shared<ExprArgument>(EXPRESSION, "_\"v\"_", buildAST("v")),
                                        false),
            patternFactory.createClause("AssignPattern",
                                        synonyms[0],
                                        std::make_shared<Argument>(WILDCARD, "_"),
                                        std::make_shared<ExprArgument>(EXPRESSION, "_\"x\"_", buildAST("x")),
                                        false),
            patternFactory.createClause("AssignPattern",
                                        synonyms[0],
                                        std::make_shared<Argument>(WILDCARD, "_"),
                                        std::make_shared<ExprArgument>(EXPRESSION, "_\"y\"_", buildAST("y")),
                                        false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Multiple Such That And Pattern Clause Test") {
        std::string queryString =
            "assign a, a1; variable v; while w;\nSelect a such that Modifies(a, v) and Parent*(w, a) such that not Follows*(1, a) pattern not a(v, _\"x\"_) pattern a1(v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory suchThatFactory;
        PatternClauseFactory patternFactory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(ASSIGN, "a1"),
            std::make_shared<Synonym>(VARIABLE, "v"),
            std::make_shared<Synonym>(WHILE, "w")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            suchThatFactory.createClause("Modifies",
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[2]),
                                         false),
            suchThatFactory.createClause("Parent*",
                                         std::make_shared<Argument>(SYNONYM, synonyms[3]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         false),
            suchThatFactory.createClause("Follows*",
                                         std::make_shared<Argument>(INT_ARG, "1"),
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         true),
            patternFactory.createClause("AssignPattern",
                                        synonyms[0],
                                        std::make_shared<Argument>(SYNONYM, synonyms[2]),
                                        std::make_shared<ExprArgument>(EXPRESSION, "_\"x\"_", buildAST("x")),
                                        true),
            patternFactory.createClause("AssignPattern",
                                        synonyms[1],
                                        std::make_shared<Argument>(SYNONYM, synonyms[2]),
                                        std::make_shared<ExprArgument>(WILDCARD, "_", std::make_shared<ExprNode>()),
                                        false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Pattern and Not Modifies Test") {
        std::string queryString = "assign a; variable v;\nSelect a such that not Modifies(a, v) pattern a(v, _\"x\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        std::shared_ptr<Query> actualQuery = queryParser.parse();
        SuchThatClauseFactory suchThatFactory;
        PatternClauseFactory patternFactory;

        std::vector<std::shared_ptr<Synonym>> synonyms = {
            std::make_shared<Synonym>(ASSIGN, "a"),
            std::make_shared<Synonym>(VARIABLE, "v")};
        std::vector<std::shared_ptr<Elem>> selectedElements = {synonyms.begin(), synonyms.begin() + 1};

        std::shared_ptr<SelectClause> selectClause = std::make_shared<SelectClause>(selectedElements, false);
        std::vector<std::shared_ptr<Clause>> constraintClauses = {
            suchThatFactory.createClause("Modifies",
                                         std::make_shared<Argument>(SYNONYM, synonyms[0]),
                                         std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                         true),
            patternFactory.createClause("AssignPattern",
                                        synonyms[0],
                                        std::make_shared<Argument>(SYNONYM, synonyms[1]),
                                        std::make_shared<ExprArgument>(EXPRESSION, "_\"x\"_", buildAST("x")),
                                        false)};
        Query expectedQuery(synonyms, selectClause, constraintClauses);

        REQUIRE((*actualQuery).equals(expectedQuery));
    }

    SECTION("Invalid And Clause Test") {
        std::string queryString = "assign a; variable v;\nSelect a such that Modifies(a, v) and pattern a(v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Missing Bracket in Such That Test") {
        std::string queryString = "stmt s; assign a;\nSelect s such that Parent* (_, 1 pattern a (_, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Missing Bracket in Pattern Test") {
        std::string queryString = "stmt s; assign a;\nSelect s pattern a (_, _ such that Parent* (_, 1)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Not Before Pattern Test") {
        std::string queryString = "assign a; variable v;\nSelect a such that Modifies(a, v) not pattern a(v, _\"x\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Not Before And in Pattern Test") {
        std::string queryString = "assign a;\nSelect a pattern a(_, _\"v\"_) not and a(_, _\"x\"_) and a(_, _\"y\"_)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }

    SECTION("Invalid Not Before Such That Test") {
        std::string queryString =
            "assign a, a1; variable v; while w;\nSelect a such that Modifies(a, v) and Parent*(w, a) not such that Follows*(1, a) pattern not a(v, _\"x\"_) pattern a1(v, _)";
        QueryTokenizer queryTokenizer = QueryTokenizer(queryString);
        std::vector<std::shared_ptr<QueryToken>> tokens = queryTokenizer.tokenize();
        QueryParser queryParser = QueryParser(tokens);

        REQUIRE_THROWS_AS(queryParser.parse(), SyntaxException);
    }
}
