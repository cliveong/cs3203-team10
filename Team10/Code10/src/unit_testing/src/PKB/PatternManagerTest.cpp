#include "catch.hpp"
#include "commons/ASTNode.h"
#include "pkb/pattern/PatternManager.h"

TEST_CASE("Pattern Manager Test - isEmpty") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
    REQUIRE(patternManager.isEmptyLhsMap());
    REQUIRE(patternManager.isEmptyRhsVector());
}

TEST_CASE("Pattern Manager Test - insertion") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
    // Insert patterns
    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 1);
    ASTNode child2 = ASTNode(type, value, 1);
    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    patternManager.insertPattern("x", root, 1);
    patternManager.insertPattern("a", root, 2);

    // Check if maps are not empty
    REQUIRE_FALSE(patternManager.isEmptyLhsMap());
    REQUIRE_FALSE(patternManager.isEmptyRhsVector());
}

TEST_CASE("Pattern Manager Test - getLhsValues") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
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

    patternManager.insertPattern("x", exprNode, 1);
    patternManager.insertPattern("cent", exprNode, 2);
    patternManager.insertPattern("x", exprNode, 3);
    patternManager.insertPattern("z", exprNode, 4);

    // Retrieve pattern values
    std::vector<int> xPatterns = patternManager.getLhsValues("x");
    std::vector<int> zPatterns = patternManager.getLhsValues("z");

    // Check if retrieved patterns are as expected
    REQUIRE(xPatterns == std::vector<int>{1, 3});
    REQUIRE_FALSE(zPatterns == std::vector<int>{2});
    REQUIRE(zPatterns == std::vector<int>{4});
}

TEST_CASE("Pattern Manager Test - getRhsIndexesFull") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
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

    patternManager.insertPattern("x", exprNode, 1);
    patternManager.insertPattern("cent", exprNode3, 2);
    patternManager.insertPattern("cent", exprNode3, 3);
    patternManager.insertPattern("cent", exprNode, 4);

    // Retrieve pattern values
    std::vector<int> exprNodeIndexes = patternManager.getRhsIndexes(exprNode, true);
    std::vector<int> exprNode3Indexes = patternManager.getRhsIndexes(exprNode3, true);

    // Check if retrieved patterns are as expected
    REQUIRE(exprNodeIndexes == std::vector<int>{0, 3});
    REQUIRE_FALSE(exprNode3Indexes == std::vector<int>{2});
    REQUIRE(exprNode3Indexes == std::vector<int>{1, 2});
}

TEST_CASE("Pattern Manager Test - containsSubTreeRhs") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
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

    patternManager.insertPattern("x", exprNode, 1);
    REQUIRE(patternManager.containsSubTreeRhs(exprNode, ASTNode("var", "y", 1)));
    REQUIRE(patternManager.containsSubTreeRhs(exprNode, ASTNode("const", "3", 1)));
    REQUIRE(patternManager.containsSubTreeRhs(exprNode, ASTNode("const", "2", 1)));
    REQUIRE_FALSE(patternManager.containsSubTreeRhs(exprNode, ASTNode("const", "7", 1)));
}

TEST_CASE("Pattern Manager Test - getRhsIndexesPartial") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
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

    patternManager.insertPattern("x", exprNode, 1);
    patternManager.insertPattern("cent", exprNode3, 2);
    patternManager.insertPattern("cent", exprNode3, 3);
    patternManager.insertPattern("cent", exprNode, 4);

    // Retrieve pattern values
    std::vector<int> exprNodeIndexes = patternManager.getRhsIndexes(ASTNode("var", "y", 1), false);
    std::vector<int> exprNode3Indexes = patternManager.getRhsIndexes(timesNode, false);

    // Check if retrieved patterns are as expected
    REQUIRE(exprNodeIndexes == std::vector<int>{0, 1, 2, 3});
    REQUIRE_FALSE(exprNode3Indexes == std::vector<int>{2});
    REQUIRE(exprNode3Indexes == std::vector<int>{1, 2});
}

TEST_CASE("Pattern Manager Test - getStmtFromIndexMap") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
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

    patternManager.insertPattern("x", exprNode, 1);
    patternManager.insertPattern("cent", exprNode, 2);
    patternManager.insertPattern("x", exprNode, 3);
    patternManager.insertPattern("z", exprNode, 4);

    // Retrieve pattern values
    int stmt = patternManager.getStmtFromIndexMap(2);
    int invalidStmt = patternManager.getStmtFromIndexMap(6);

    // Check if retrieved patterns are as expected
    REQUIRE(stmt == 3);
    REQUIRE(invalidStmt == 0);
}

TEST_CASE("Pattern Manager Test - patterns that do not exist") {
    // Create a PatternManager instance
    PatternManager<std::string, ASTNode> patternManager;
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

    patternManager.insertPattern("x", exprNode, 1);
    patternManager.insertPattern("cent", exprNode, 4);

    // Retrieve pattern values
    std::vector<int> exprNodeIndexes = patternManager.getRhsIndexes(exprNode3, true);
    std::vector<int> exprNode3Indexes = patternManager.getRhsIndexes(divNode2, false);

    // Check if retrieved patterns are as expected
    REQUIRE(exprNodeIndexes.empty());
    REQUIRE_FALSE(exprNode3Indexes == std::vector<int>{2});
    REQUIRE(exprNode3Indexes.empty());
}