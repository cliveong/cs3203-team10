#include "catch.hpp"
#include "commons/ASTNode.h"
#include "pkb/Storage.h"
#include "pkb/WriteFacade.h"

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

TEST_CASE("WriteFacade Test storeAssign() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    writeFacade.storeAssign(1);
    std::vector<int> entitiesSet = storage->getAssign();

    REQUIRE(entitiesSet.size() == 1);

    writeFacade.storeAssign(1);
    entitiesSet = storage->getAssign();
    // does not allow duplicates in set
    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storeConstant() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    writeFacade.storeConstant("2");
    std::vector<std::string> entitiesSet = storage->getConstant();

    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storeIf() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    writeFacade.storeIf(21);
    std::vector<int> entitiesSet = storage->getIf();

    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storeStmt() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    writeFacade.storeStmt(21);
    std::vector<int> entitiesSet = storage->getStmt();

    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storeWhile() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    writeFacade.storeWhile(21);
    std::vector<int> entitiesSet = storage->getWhile();

    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storeProcedureName() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    std::string varName = "procedureName";
    writeFacade.storeProcedureName(varName);
    std::vector<std::string> entitiesSet = storage->getProcedure();

    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storeVariable() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    std::string varName = "x";
    writeFacade.storeVariable(varName);
    std::vector<std::string> entitiesSet = storage->getVariable();

    REQUIRE(entitiesSet.size() == 1);
}

TEST_CASE("WriteFacade Test storePrintStmt() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    std::string varName = "test";
    writeFacade.storePrintStmt(21, varName);
    std::vector<int> printSset = storage->getPrintS();

    REQUIRE(printSset.size() == 1);
}

TEST_CASE("WriteFacade Test storeReadStmt() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    std::string varName = "test";
    writeFacade.storeReadStmt(21, varName);
    std::vector<int> readSset = storage->getReadS();

    REQUIRE(readSset.size() == 1);
}

TEST_CASE("Write Facade Test storeFollows() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeFollows(root, {child, child2});

    bool followsKeys = storage->hasFollows();
    REQUIRE(followsKeys);

    std::vector<int> followsKey = storage->getFollows(1);
    REQUIRE(followsKey.size() == 2);
}

TEST_CASE("Write Facade Test storeParent() function") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);
    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));
    writeFacade.storeParents(root, {child, child2});
    std::vector<int> parentKeys = storage->getParents(1);
    REQUIRE(parentKeys.size() == 2);

    writeFacade.storeParents(root, {child, child2});
    bool hasparentKeys = storage->hasParents();
    REQUIRE(hasparentKeys);
}

TEST_CASE("Write Facade Test storeAssignPattern()") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);

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

    writeFacade.storeAssignPattern(ASTNode("var", "x", 2), exprNode, 1);
    writeFacade.storeAssignPattern(ASTNode("var", "cent", 2), exprNode3, 3);

    SECTION("get LHS values") {
        std::string x = "x";
        std::string cent = "cent";
        std::vector<int> lhsValues = storage->getAssignPatternLhs(x);
        std::vector<int> lhsValues2 = storage->getAssignPatternLhs(cent);
        REQUIRE(lhsValues.size() == 1);
        REQUIRE(lhsValues2.size() == 1);
    }

    SECTION("get rhs values - partial match") {
        ASTNode yNode("var", "y", 1);
        std::vector<int> rhsConstant = storage->getAssignPatternRhs(yNode);
        REQUIRE(rhsConstant.size() == 2);
    }

    SECTION("get rhs values - full match") {
        ASTNode yNode("var", "y", 1);
        std::vector<int> rhsValues = storage->getAssignPatternRhsFull(exprNode3);
        REQUIRE(rhsValues.size() == 1);
    }
}

TEST_CASE("Write Facade Test storeIfPattern()") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);

    std::vector<std::string> ifVariables = {"x", "juice", "X", "k"};
    std::vector<std::string> ifVariables2 = {"x", "X"};
    writeFacade.storeIfPattern(ifVariables, 1);
    writeFacade.storeIfPattern(ifVariables2, 3);

    SECTION("get LHS values - with results") {
        std::string x = "x";
        std::string X = "X";
        std::string juice = "juice";

        std::vector<int> xValues = storage->getIfPattern(x);
        std::vector<int> x_Values = storage->getIfPattern(X);
        std::vector<int> juiceValues = storage->getIfPattern(juice);
        REQUIRE(is_same_vector(xValues, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(x_Values, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(juiceValues, std::vector<int>{1}));
    }

    SECTION("get LHS values - no results") {
        std::string nonexistent = "nonexistent";
        std::string emptyString;

        std::vector<int> xValues = storage->getIfPattern(nonexistent);
        std::vector<int> x_Values = storage->getIfPattern(emptyString);
        REQUIRE(xValues.empty());
        REQUIRE(x_Values.empty());
    }
}

TEST_CASE("Write Facade Test storeWhilePattern()") {
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade(storage);

    std::vector<std::string> var = {"x", "juice", "X", "k"};
    std::vector<std::string> var2 = {"x", "X"};
    writeFacade.storeWhilePattern(var, 1);
    writeFacade.storeWhilePattern(var2, 3);

    SECTION("get LHS values - with results") {
        std::string x = "x";
        std::string X = "X";
        std::string juice = "juice";

        std::vector<int> xValues = storage->getWhilePattern(x);
        std::vector<int> x_Values = storage->getWhilePattern(X);
        std::vector<int> juiceValues = storage->getWhilePattern(juice);
        REQUIRE(is_same_vector(xValues, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(x_Values, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(juiceValues, std::vector<int>{1}));
    }

    SECTION("get LHS values - no results") {
        std::string nonexistent = "nonexistent";
        std::string emptyString;

        std::vector<int> xValues = storage->getWhilePattern(nonexistent);
        std::vector<int> x_Values = storage->getWhilePattern(emptyString);
        REQUIRE(xValues.empty());
        REQUIRE(x_Values.empty());
    }
}
