#include <algorithm>
#include <iostream>
#include <vector>

#include "catch.hpp"
#include "pkb/ReadFacade.h"
#include "pkb/Storage.h"

template <typename T>
bool is_same_vector(const std::vector<T>& v1, const std::vector<T>& v2) {
    // Check if the vectors have the same size
    if (v1.size() != v2.size()) {
        return false;
    }

    // Sort both vectors
    std::vector<T> sortedV1 = v1;
    std::vector<T> sortedV2 = v2;
    std::sort(sortedV1.begin(), sortedV1.end());
    std::sort(sortedV2.begin(), sortedV2.end());

    // Check if the sorted vectors are the same
    return sortedV1 == sortedV2;
}

template <typename T>
void printVectorWithBrackets(const std::vector<T>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

ASTNode createASTNode(const std::string& value) {
    return ASTNode("", value, 0);
}

ASTNode createASTNode(const int& value) {
    return ASTNode("", "", value);
}

std::vector<ASTNode> createASTNodesFromStringVector(const std::vector<std::string>& names) {
    std::vector<ASTNode> nodes;
    for (const auto& name : names) {
        nodes.emplace_back("", name, 0);
    }
    return nodes;
}

std::vector<ASTNode> createASTNodesFromIntVector(const std::vector<int>& ids) {
    std::vector<ASTNode> nodes;
    for (int id : ids) {
        nodes.emplace_back("", std::to_string(id), id);
    }
    return nodes;
}

TEST_CASE("Test Parent Storage") {
    // Create instances of Follow and Parent and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();

    // Create a ReadFacade instance by passing the shared pointers to the constructor
    ReadFacade readManager(storageManager);
    std::vector<int> myVector = {2, 3, 4};
    storageManager->storeParents(createASTNode(1), createASTNodesFromIntVector(myVector));
    storageManager->storeFollows(createASTNode(1), createASTNodesFromIntVector(std::vector<int>{1, 2, 3, 4, 5}));

    std::vector<int> myVector2 = {3, 4};
    std::vector<int> myVector3 = {4};
    storageManager->storeParents(createASTNode(2), createASTNodesFromIntVector(myVector2));
    storageManager->storeParents(createASTNode(3), createASTNodesFromIntVector(myVector3));

    // Use the getParents methods to interact with the managers
    std::vector<int> parentsResult = readManager.getParents(1, false);

    REQUIRE(parentsResult == myVector);

    std::vector<int> getParentsS2Result = readManager.getParentsS2(4, false);
    std::vector<int> parentsS2Reference = {1, 2, 3};
    REQUIRE(getParentsS2Result == parentsS2Reference);

    bool hasParents = readManager.hasParents(false);
    REQUIRE(hasParents);
}

TEST_CASE("Test Follow Storage") {
    // Create instances of Follow and Parent and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();

    // Create a ReadFacade instance by passing the shared pointers to the constructor
    ReadFacade readManager(storageManager);
    std::vector<int> myVector = {2, 3, 4};
    storageManager->storeParents(createASTNode(1), createASTNodesFromIntVector(std::vector<int>{1, 2, 3, 4, 5}));
    storageManager->storeFollows(createASTNode(1), createASTNodesFromIntVector(myVector));

    std::vector<int> myVector2 = {3, 4};
    std::vector<int> myVector3 = {4};
    storageManager->storeFollows(createASTNode(2), createASTNodesFromIntVector(myVector2));
    storageManager->storeFollows(createASTNode(3), createASTNodesFromIntVector(myVector3));

    // Use the getFollows and getParents methods to interact with the managers
    std::vector<int> followsResult = readManager.getFollows(1, true);

    REQUIRE(followsResult == myVector);

    std::vector<int> getFollowsS2Result = readManager.getFollowsS2(4, true);
    std::vector<int> followsS2Reference = {1, 2, 3};
    REQUIRE(is_same_vector(getFollowsS2Result, followsS2Reference));

    std::vector<int> getFollowsS2Result2 = readManager.getFollowsS2(3, true);
    std::vector<int> followsS2Reference2 = {1, 2};
    REQUIRE(is_same_vector(getFollowsS2Result2, followsS2Reference2));

    std::vector<int> getFollowsS2Result3 = readManager.getFollowsS2(2, true);
    std::vector<int> followsS2Reference3 = {1};
    REQUIRE(is_same_vector(getFollowsS2Result3, followsS2Reference3));

    bool hasFollows = readManager.hasFollows();
    std::vector<std::vector<int>> allfollowsReference2 = {{1, 2, 3, 4}, {2, 3, 4}, {3, 4}};
    REQUIRE(hasFollows);
}

TEST_CASE("Test out of range") {
    // Create instances of Follow and Parent and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();

    // Create a ReadFacade instance by passing the shared pointers to the constructor
    ReadFacade readManager(storageManager);

    std::vector<int> getAllFollows = readManager.getFollows(10, true);
    std::vector<int> allfollowsReference = {};
    REQUIRE(getAllFollows == allfollowsReference);
}

TEST_CASE("Read Assign Pattern Test") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);

    SECTION("Initially empty") {
        REQUIRE(readManager.getAssignPatternLhs("x").empty());
        ASTNode yNode("var", "y", 1);
        REQUIRE(readManager.getAssignPatternRhs(yNode, false).empty());
    }

    // std::string input = " procedure main { x = y + 3 / 2; }";
    ASTNode exprNode("+", "+", 1);
    ASTNode divNode("/", "/", 1);
    divNode.addChild(std::make_shared<ASTNode>("const", "3", 1));
    divNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    exprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    exprNode.addChild(std::make_shared<ASTNode>(divNode));
    // std::string input = " procedure main { cent = v + x * y + z / t; }";
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
    storageManager->storeAssignPattern("x", exprNode, 1);
    storageManager->storeAssignPattern("cent", exprNode3, 5);
    storageManager->storeAssignPattern("cent", exprNode, 6);

    SECTION("Get LHS values") {
        std::vector<int> lhsValues = readManager.getAssignPatternLhs("x");
        REQUIRE(lhsValues.size() == 1);
        std::vector<int> lhsValues2 = readManager.getAssignPatternLhs("cent");
        REQUIRE(lhsValues2.size() == 2);
        std::vector<int> lhsValues3 = readManager.getAssignPatternLhs("nonexistent");
        REQUIRE(lhsValues3.empty());
    }

    SECTION("Get RHS partial match") {
        ASTNode yNode("var", "y", 1);
        ASTNode constNode("const", "2", 1);

        std::vector<int> rhsValues = readManager.getAssignPatternRhs(yNode, false);
        std::vector<int> rhsValues2 = readManager.getAssignPatternRhs(divNode, false);
        std::vector<int> rhsConstant = readManager.getAssignPatternRhs(constNode, false);
        REQUIRE(rhsValues.size() == 3);
        REQUIRE(rhsValues2.size() == 2);
        REQUIRE(rhsConstant.size() == 2);
    }

    SECTION("Get RHS full match") {
        ASTNode yNode("var", "y", 1);

        std::vector<int> rhsValues = readManager.getAssignPatternRhs(yNode, true);
        std::vector<int> rhsValues2 = readManager.getAssignPatternRhs(exprNode, true);
        std::vector<int> rhsValues3 = readManager.getAssignPatternRhs(exprNode3, true);
        REQUIRE(rhsValues.empty());
        REQUIRE(rhsValues2.size() == 2);
        REQUIRE(rhsValues3.size() == 1);
    }

    SECTION("Get both lhs and rhs full match") {
        ASTNode yNode("var", "y", 1);

        std::vector<int> rhsValues = readManager.getAssignPattern("x", yNode, true);
        std::vector<int> rhsValues2 = readManager.getAssignPattern("x", exprNode, true);
        std::vector<int> rhsValues3 = readManager.getAssignPattern("cent", exprNode3, true);
        REQUIRE(rhsValues.empty());
        REQUIRE(rhsValues2.size() == 1);
        REQUIRE(rhsValues3.size() == 1);
    }
    SECTION("Get both lhs and rhs partial match") {
        ASTNode yNode("var", "y", 1);

        std::vector<int> rhsValues = readManager.getAssignPattern("cent", yNode, false);
        std::vector<int> rhsValues2 = readManager.getAssignPattern("x", divNode, false);
        std::vector<int> rhsValues3 = readManager.getAssignPattern("cent", divNode2, false);
        std::vector<int> rhsValues4 = readManager.getAssignPattern("x", divNode2, false);
        std::vector<int> rhsValues5 = readManager.getAssignPattern("juice", divNode2, false);
        REQUIRE(rhsValues.size() == 2);
        REQUIRE(rhsValues2.size() == 1);
        REQUIRE(rhsValues3.size() == 1);
        REQUIRE(rhsValues4.empty());
        REQUIRE(rhsValues5.empty());
    }
}

TEST_CASE("Read IfPattern Test") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    ReadFacade readManager(storage);

    std::vector<std::string> ifVariables = {"x", "juice", "X", "k"};
    std::vector<std::string> ifVariables2 = {"x", "X"};
    storage->storeIfPattern(ifVariables, 1);
    storage->storeIfPattern(ifVariables2, 3);

    SECTION("get LHS values - with results") {
        std::string x = "x";
        std::string X = "X";
        std::string juice = "juice";

        std::vector<int> xValues = readManager.getIfPattern(x);
        std::vector<int> x_Values = readManager.getIfPattern(X);
        std::vector<int> juiceValues = readManager.getIfPattern(juice);
        REQUIRE(is_same_vector(xValues, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(x_Values, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(juiceValues, std::vector<int>{1}));
    }

    SECTION("get LHS values - no results") {
        std::string nonexistent = "nonexistent";
        std::string emptyString;

        std::vector<int> xValues = readManager.getIfPattern(nonexistent);
        std::vector<int> x_Values = readManager.getIfPattern(emptyString);
        REQUIRE(xValues.empty());
        REQUIRE(x_Values.empty());
    }
}

TEST_CASE("Read WhilePattern Test") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    ReadFacade readManager(storage);

    std::vector<std::string> var = {"x", "juice", "X", "k"};
    std::vector<std::string> var2 = {"x", "X"};
    storage->storeWhilePattern(var, 1);
    storage->storeWhilePattern(var2, 3);

    SECTION("get LHS values - with results") {
        std::string x = "x";
        std::string X = "X";
        std::string juice = "juice";

        std::vector<int> xValues = readManager.getWhilePattern(x);
        std::vector<int> x_Values = readManager.getWhilePattern(X);
        std::vector<int> juiceValues = readManager.getWhilePattern(juice);
        REQUIRE(is_same_vector(xValues, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(x_Values, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(juiceValues, std::vector<int>{1}));
    }

    SECTION("get LHS values - no results") {
        std::string nonexistent = "nonexistent";
        std::string emptyString;

        std::vector<int> xValues = readManager.getWhilePattern(nonexistent);
        std::vector<int> x_Values = readManager.getWhilePattern(emptyString);
        REQUIRE(xValues.empty());
        REQUIRE(x_Values.empty());
    }
}