#include "catch.hpp"
#include "pkb/pattern/AssignPatternManager.h"

TEST_CASE("Assign Pattern Manager Test - getAllMatch() full and valid") {
    AssignPatternManager assignPatternManager;
    // Insert patterns
    // std::string input = " procedure main { x = y + 3 / 2; }";
    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    ASTNode exprNode("+", "+", 1);
    ASTNode divNode("/", "/", 1);
    divNode.addChild(std::make_shared<ASTNode>("const", "3", 1));
    divNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    exprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    exprNode.addChild(std::make_shared<ASTNode>(divNode));
    // std::string input = " procedure main { cent = v + x * y + z / t; }";
    ASTNode assignNode2("assign", "", 2);
    assignNode.addChild(std::make_shared<ASTNode>("var", "cent", 2));
    ASTNode exprNode2("+", "+", 2);
    ASTNode exprNode3("+", "+", 2);
    ASTNode divNode2("/", "/", 2);
    ASTNode timesNode("*", "*", 2);
    divNode2.addChild(std::make_shared<ASTNode>("var", "z", 2));
    divNode2.addChild(std::make_shared<ASTNode>("var", "t", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "x", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    exprNode2.addChild(std::make_shared<ASTNode>("var", "v", 2));
    exprNode2.addChild(std::make_shared<ASTNode>(timesNode));
    exprNode3.addChild(std::make_shared<ASTNode>(exprNode2));
    exprNode3.addChild(std::make_shared<ASTNode>(divNode2));

    assignPatternManager.insertPattern("x", exprNode, 1);
    assignPatternManager.insertPattern("cent", exprNode3, 2);
    assignPatternManager.insertPattern("cent", exprNode3, 78);
    assignPatternManager.insertPattern("cent", exprNode, 4);

    SECTION("Get all full matches for existing patterns") {
        std::vector<int> matches = assignPatternManager.getAllMatch("x", exprNode, true);
        REQUIRE(matches == std::vector<int>{1});

        std::vector<int> matches2 = assignPatternManager.getAllMatch("cent", exprNode, true);
        REQUIRE(matches2 == std::vector<int>{4});

        std::vector<int> matches3 = assignPatternManager.getAllMatch("cent", exprNode3, true);
        REQUIRE(matches3 == std::vector<int>{2, 78});
    }
}

TEST_CASE("Assign Pattern Manager Test - getAllMatch() partial and valid") {
    AssignPatternManager assignPatternManager;
    // Insert patterns
    // std::string input = " procedure main { x = y + 3 / 2; }";
    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    ASTNode exprNode("+", "+", 1);
    ASTNode divNode("/", "/", 1);
    divNode.addChild(std::make_shared<ASTNode>("const", "3", 1));
    divNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    exprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    exprNode.addChild(std::make_shared<ASTNode>(divNode));
    // std::string input = " procedure main { cent = v + x * y + z / t; }";
    ASTNode assignNode2("assign", "", 2);
    assignNode.addChild(std::make_shared<ASTNode>("var", "cent", 2));
    ASTNode exprNode2("+", "+", 2);
    ASTNode exprNode3("+", "+", 2);
    ASTNode divNode2("/", "/", 2);
    ASTNode timesNode("*", "*", 2);
    divNode2.addChild(std::make_shared<ASTNode>("var", "z", 2));
    divNode2.addChild(std::make_shared<ASTNode>("var", "t", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "x", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    exprNode2.addChild(std::make_shared<ASTNode>("var", "v", 2));
    exprNode2.addChild(std::make_shared<ASTNode>(timesNode));
    exprNode3.addChild(std::make_shared<ASTNode>(exprNode2));
    exprNode3.addChild(std::make_shared<ASTNode>(divNode2));

    assignPatternManager.insertPattern("x", exprNode, 1);
    assignPatternManager.insertPattern("cent", exprNode3, 2);
    assignPatternManager.insertPattern("cent", exprNode3, 78);
    assignPatternManager.insertPattern("cent", exprNode, 4);

    SECTION("Get all partial matches for existing patterns") {
        std::vector<int> matches = assignPatternManager.getAllMatch("cent", divNode2, false);
        REQUIRE(matches == std::vector<int>{2, 78});

        std::vector<int> matches2 = assignPatternManager.getAllMatch("x", ASTNode("const", "3", 1), false);
        REQUIRE(matches2 == std::vector<int>{1});

        std::vector<int> matches3 = assignPatternManager.getAllMatch("cent", exprNode2, false);
        REQUIRE(matches3 == std::vector<int>{2, 78});

        std::vector<int> matches4 = assignPatternManager.getAllMatch("cent", ASTNode("var", "y", 2), false);
        REQUIRE(matches4 == std::vector<int>{2, 4, 78});
    }
}

TEST_CASE("Assign Pattern Manager Test - getAllMatch() invalid") {
    AssignPatternManager assignPatternManager;
    // Insert patterns
    // std::string input = " procedure main { x = y + 3 / 2; }";
    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    ASTNode exprNode("+", "+", 1);
    ASTNode divNode("/", "/", 1);
    divNode.addChild(std::make_shared<ASTNode>("const", "3", 1));
    divNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    exprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    exprNode.addChild(std::make_shared<ASTNode>(divNode));
    // std::string input = " procedure main { cent = v + x * y + z / t; }";
    ASTNode assignNode2("assign", "", 2);
    assignNode.addChild(std::make_shared<ASTNode>("var", "cent", 2));
    ASTNode exprNode2("+", "+", 2);
    ASTNode exprNode3("+", "+", 2);
    ASTNode divNode2("/", "/", 2);
    ASTNode timesNode("*", "*", 2);
    divNode2.addChild(std::make_shared<ASTNode>("var", "z", 2));
    divNode2.addChild(std::make_shared<ASTNode>("var", "t", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "x", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    exprNode2.addChild(std::make_shared<ASTNode>("var", "v", 2));
    exprNode2.addChild(std::make_shared<ASTNode>(timesNode));
    exprNode3.addChild(std::make_shared<ASTNode>(exprNode2));
    exprNode3.addChild(std::make_shared<ASTNode>(divNode2));

    assignPatternManager.insertPattern("x", exprNode, 1);
    assignPatternManager.insertPattern("cent", exprNode3, 2);
    assignPatternManager.insertPattern("cent", exprNode3, 78);
    assignPatternManager.insertPattern("cent", exprNode, 4);

    SECTION("Get all matches for non-existent patterns") {
        std::vector<int> matches = assignPatternManager.getAllMatch("nonexistent", exprNode2, true);
        REQUIRE(matches.empty());
        std::vector<int> matches2 = assignPatternManager.getAllMatch("nonexistent", assignNode, false);
        REQUIRE(matches2.empty());
    }

    SECTION("Get all matches for one existing and one non-existent pattern") {
        std::vector<int> matches = assignPatternManager.getAllMatch("x", exprNode2, true);
        REQUIRE(matches.empty());
        std::vector<int> matches2 = assignPatternManager.getAllMatch("hello", exprNode2, false);
        REQUIRE(matches2.empty());
    }

    SECTION("Get all matches for both existing pattern but different stmt") {
        std::vector<int> matches = assignPatternManager.getAllMatch("x", exprNode3, true);
        REQUIRE(matches.empty());
        std::vector<int> matches2 = assignPatternManager.getAllMatch("x", divNode2, false);
        REQUIRE(matches2.empty());
    }
}

TEST_CASE("Assign Pattern Manager Test - getRhsValues") {
    AssignPatternManager assignPatternManager;
    // Insert patterns
    // std::string input = " procedure main { x = y + 3 / 2; }";
    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    ASTNode exprNode("+", "+", 1);
    ASTNode divNode("/", "/", 1);
    divNode.addChild(std::make_shared<ASTNode>("const", "3", 1));
    divNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    exprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    exprNode.addChild(std::make_shared<ASTNode>(divNode));
    // std::string input = " procedure main { cent = v + x * y + z / t; }";
    ASTNode assignNode2("assign", "", 2);
    assignNode.addChild(std::make_shared<ASTNode>("var", "cent", 2));
    ASTNode exprNode2("+", "+", 2);
    ASTNode exprNode3("+", "+", 2);
    ASTNode divNode2("/", "/", 2);
    ASTNode timesNode("*", "*", 2);
    divNode2.addChild(std::make_shared<ASTNode>("var", "z", 2));
    divNode2.addChild(std::make_shared<ASTNode>("var", "t", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "x", 2));
    timesNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    exprNode2.addChild(std::make_shared<ASTNode>("var", "v", 2));
    exprNode2.addChild(std::make_shared<ASTNode>(timesNode));
    exprNode3.addChild(std::make_shared<ASTNode>(exprNode2));
    exprNode3.addChild(std::make_shared<ASTNode>(divNode2));

    assignPatternManager.insertPattern("x", exprNode, 1);
    assignPatternManager.insertPattern("cent", exprNode3, 2);
    assignPatternManager.insertPattern("cent", exprNode3, 78);
    assignPatternManager.insertPattern("cent", exprNode, 4);

    SECTION("Get full rhs matches") {
        std::vector<int> matches = assignPatternManager.getRhsValues(exprNode, true);
        REQUIRE(matches == std::vector<int>{1, 4});

        std::vector<int> matches3 = assignPatternManager.getRhsValues(exprNode3, true);
        REQUIRE(matches3 == std::vector<int>{2, 78});
    }
    SECTION("Get full rhs matches - invalid") {
        std::vector<int> matches3 = assignPatternManager.getRhsValues(exprNode2, true);
        REQUIRE(matches3.empty());
    }

    SECTION("Get partial rhs matches") {
        std::vector<int> matches = assignPatternManager.getRhsValues(divNode, false);
        REQUIRE(matches == std::vector<int>{1, 4});

        std::vector<int> matches2 = assignPatternManager.getRhsValues(divNode2, false);
        REQUIRE(matches2 == std::vector<int>{2, 78});

        ASTNode yNode("var", "y", 1);
        std::vector<int> matches3 = assignPatternManager.getRhsValues(yNode, false);
        REQUIRE(matches3 == std::vector<int>{1, 2, 78, 4});
    }
    SECTION("Get partial rhs matches - nonexistent invalid") {
        ASTNode invalidNode("var", "hello", 1);
        std::vector<int> matches3 = assignPatternManager.getRhsValues(invalidNode, false);
        REQUIRE(matches3.empty());
    }

    SECTION("Get partial rhs matches - invalid subtree") {
        // "y+z*t"
        ASTNode invalidNode("+", "+", 1);
        invalidNode.addChild(std::make_shared<ASTNode>("var", "v", 1));
        ASTNode exprNode4("*", "*", 1);
        exprNode4.addChild(std::make_shared<ASTNode>("var", "z", 1));
        exprNode4.addChild(std::make_shared<ASTNode>("var", "t", 1));
        invalidNode.addChild(std::make_shared<ASTNode>(exprNode4));

        std::vector<int> matches = assignPatternManager.getRhsValues(invalidNode, false);
        REQUIRE(matches.empty());
    }
}
