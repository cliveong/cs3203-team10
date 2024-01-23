#include <algorithm>
#include <vector>

#include "catch.hpp"
#include "pkb/ReadFacade.h"
#include "pkb/Storage.h"
#include "pkb/WriteFacade.h"
#include "pkb/relationship_managers/RelationshipManager.h"

ASTNode createASTNodes(const std::string& value) {
    return ASTNode("", value, 0);
}

ASTNode createASTNodes(const int& value) {
    return ASTNode("", "", value);
}

std::vector<ASTNode> createASTNodeFromStringVector(const std::vector<std::string>& names) {
    std::vector<ASTNode> nodes;
    for (const auto& name : names) {
        nodes.emplace_back("", name, 0);
    }
    return nodes;
}

std::vector<ASTNode> createASTNodeFromIntVector(const std::vector<int>& ids) {
    std::vector<ASTNode> nodes;
    for (int id : ids) {
        nodes.emplace_back("", std::to_string(id), id);
    }
    return nodes;
}

CFGNode createCFGNode(int headNodeStmtNum, const std::vector<int>& successorStmtNums) {
    // Find or create the head CFGNode
    CFGNode headNode = CFGNode(headNodeStmtNum);

    for (int stmtNum : successorStmtNums) {
        // Find or create successor CFGNode
        std::shared_ptr<CFGNode> successorNode = std::make_shared<CFGNode>(stmtNum);
        // Add this successor to the headNode's successors
        headNode.addSuccessor(successorNode);
    }
    return headNode;
}

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

bool is_same_nested_vector(const std::vector<std::vector<std::string>>& vector1,
                           const std::vector<std::vector<std::string>>& vector2) {
    // Function to compare two vectors of strings
    auto compareVectors = [](const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
        return std::lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
    };

    // Make copies of the vectors and sort the inner vectors
    std::vector<std::vector<std::string>> sortedVector1 = vector1;
    std::vector<std::vector<std::string>> sortedVector2 = vector2;

    for (auto& v : sortedVector1) {
        std::sort(v.begin(), v.end());
    }

    for (auto& v : sortedVector2) {
        std::sort(v.begin(), v.end());
    }

    // Sort the outer vectors
    std::sort(sortedVector1.begin(), sortedVector1.end(), compareVectors);
    std::sort(sortedVector2.begin(), sortedVector2.end(), compareVectors);

    // Compare the sorted vectors
    return sortedVector1 == sortedVector2;
}

// ai-gen (chatgpt, 2)
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

void printNestedVector(const std::vector<std::vector<int>>& nestedVector) {
    std::cout << "{";
    for (const auto& innerVector : nestedVector) {
        std::cout << "{";
        for (const auto& value : innerVector) {
            std::cout << value;
            if (&value != &innerVector.back()) {
                std::cout << ", ";
            }
        }
        std::cout << "}";
        if (&innerVector != &nestedVector.back()) {
            std::cout << ", ";
        }
    }
    std::cout << "}";
}
// ai end

TEST_CASE("Read Write Assign") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    writeFacade.storeAssign(2);
    std::vector<int> getAssign = readManager.getAssign();
    REQUIRE(getAssign == std::vector<int>{2});
}

TEST_CASE("Read Write Constant") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    writeFacade.storeConstant("3");
    std::vector<std::string> getConstant = readManager.getConstant();
    REQUIRE(getConstant == std::vector<std::string>{"3"});
}

TEST_CASE("Read Write if") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    writeFacade.storeIf(4);
    std::vector<int> getIf = readManager.getIf();
    REQUIRE(getIf == std::vector<int>{4});
}

TEST_CASE("Read Write Print SV") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    std::string varName = "tbate";
    writeFacade.storePrintStmt(5, varName);
    std::vector<int> getPrintS = readManager.getPrintS();
    //    std::vector<std::string> getPrintV = readManager.getPrintV();
    REQUIRE(getPrintS == std::vector<int>{5});
    //    REQUIRE(getPrintV == std::vector<std::string>{"tbate"});
}

TEST_CASE("Read Write Proc") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    std::string varName = "tbate2";
    writeFacade.storeProcedureName(varName);
    std::vector<std::string> getProcedure = readManager.getProcedure();
    REQUIRE(getProcedure == std::vector<std::string>{"tbate2"});
}

// Insert into ReadV is failing
TEST_CASE("Read Write Read SV") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    std::string varName = "tbate3";
    writeFacade.storeReadStmt(6, varName);
    std::vector<int> getReadS = readManager.getReadS();
    //    std::vector<std::string> getReadV = readManager.getReadV();
    REQUIRE(getReadS.size() == 1);
    REQUIRE(getReadS == std::vector<int>{6});
    //    REQUIRE(getReadV == std::vector<std::string>{"tbate3"});
}

TEST_CASE("Read Write Stmt") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    writeFacade.storeStmt(7);
    std::vector<int> getStmt = readManager.getStmt();
    REQUIRE(getStmt == std::vector<int>{7});
}

TEST_CASE("Read Write Variable") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    std::string varName = "tbate4";
    writeFacade.storeVariable(varName);
    std::vector<std::string> getVariable = readManager.getVariable();
    REQUIRE(getVariable == std::vector<std::string>{"tbate4"});
}

TEST_CASE("Read Write While") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);

    WriteFacade writeFacade(storageManager);
    writeFacade.storeWhile(8);
    std::vector<int> getWhile = readManager.getWhile();
    REQUIRE(getWhile == std::vector<int>{8});
}

TEST_CASE("Read Write Follows") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "assign";
    std::string value = "x";
    std::shared_ptr<ASTNode> root = std::make_shared<ASTNode>(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root->addChild(std::make_shared<ASTNode>(child));
    root->addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeFollows(*root, {child, child2});

    std::vector<int> expected = {2, 3};
    std::vector<int> getFollows = readManager.getFollows(1, true);
    REQUIRE(getFollows.size() == 2);
    REQUIRE(is_same_vector(expected, getFollows));
    std::vector<int> expected2 = {2};
    std::vector<int> getFollows2 = readManager.getFollows(1, false);
    REQUIRE(getFollows2.size() == 1);
    REQUIRE(is_same_vector(expected2, getFollows2));
}

TEST_CASE("Read Write Follows 2") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeFollows(root, {child, child2});

    std::vector<int> getFollows = readManager.getFollows(1, true);
    REQUIRE(getFollows.size() == 2);

    ASTNode root2 = ASTNode(type, value, 1);
    ASTNode child3 = ASTNode(type, value, 4);
    ASTNode child4 = ASTNode(type, value, 5);

    root2.addChild(std::make_shared<ASTNode>(child3));
    root2.addChild(std::make_shared<ASTNode>(child4));

    writeFacade.storeFollows(root2, {child3, child4});
    std::vector<int> getFollows2 = readManager.getFollows(1, true);
    REQUIRE(getFollows2.size() == 4);
    REQUIRE(is_same_vector(getFollows2, std::vector<int>{2, 3, 4, 5}));
}

TEST_CASE("Read Write FollowsS2") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<int> followsKeys = readManager.getFollowsS2(2, true);
    REQUIRE(followsKeys.size() == 0);

    readManager.clearCache();

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeFollows(root, {child, child2});

    followsKeys = readManager.getFollowsS2(2, true);
    REQUIRE(followsKeys.size() == 1);
}

TEST_CASE("Read Write AllFollows") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    bool hasFollows = readManager.hasFollows();
    REQUIRE(!hasFollows);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeFollows(root, {child, child2});

    hasFollows = readManager.hasFollows();
    REQUIRE(hasFollows);
    hasFollows = readManager.hasFollowsStar();
    REQUIRE(hasFollows);
}

TEST_CASE("Read Write Parents") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParents(root, {child, child2});
    std::vector<int> getParent = readManager.getParents(1, false);
    std::vector<int> expected = {2, 3};
    REQUIRE(getParent == expected);
}

TEST_CASE("Read Write ParentsS2") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<int> parentsKeys = readManager.getParentsS2(3, false);
    REQUIRE(parentsKeys.size() == 0);

    readManager.clearCache();

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParents(root, {child, child2});
    std::vector<int> getParent = readManager.getParentsS2(3, false);
    std::vector<int> expected = {1};
    REQUIRE(getParent == expected);
}

TEST_CASE("Read Write AllParents") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    bool hasParent = readManager.hasParents(false);
    REQUIRE(!hasParent);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParents(root, {child, child2});
    hasParent = readManager.hasParents(false);
    REQUIRE(hasParent);
}

TEST_CASE("Read Write ParentStar") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 5);
    ASTNode child2 = ASTNode(type, value, 6);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParentStar(root, {child, child2});
    std::vector<int> getParent = readManager.getParents(1, true);
    std::vector<int> expected = {5, 6};
    REQUIRE(getParent == expected);
}

TEST_CASE("Read Write ParentsS2Star") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<int> parentsKeys = readManager.getParentsS2(7, true);
    REQUIRE(parentsKeys.size() == 0);

    readManager.clearCache();

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 7);
    ASTNode child2 = ASTNode(type, value, 8);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParentStar(root, {child, child2});
    std::vector<int> getParent = readManager.getParentsS2(7, true);
    std::vector<int> expected = {1};
    REQUIRE(getParent == expected);
}

TEST_CASE("Read Write AllParentsStar") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    bool hasParent = readManager.hasParents(true);
    REQUIRE(!hasParent);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 123);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParentStar(root, {child, child2});
    hasParent = readManager.hasParents(true);
    REQUIRE(hasParent);
}

TEST_CASE("Read Write Assign Pattern Test") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

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
    writeFacade.storeAssignPattern(ASTNode("var", "juice", 4), exprNode2, 5);
    writeFacade.storeAssignPattern(ASTNode("var", "cent", 6), exprNode2, 3);

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
        REQUIRE(rhsValues.size() == 4);
        REQUIRE(rhsValues2.size() == 1);
        REQUIRE(rhsConstant.size() == 1);
    }

    SECTION("Get RHS full match") {
        ASTNode yNode("var", "y", 1);

        std::vector<int> rhsValues = readManager.getAssignPatternRhs(yNode, true);
        std::vector<int> rhsValues2 = readManager.getAssignPatternRhs(exprNode, true);
        std::vector<int> rhsValues3 = readManager.getAssignPatternRhs(exprNode3, true);
        REQUIRE(rhsValues.empty());
        REQUIRE(rhsValues2.size() == 1);
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

TEST_CASE("Read Write usesS") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "usesAPn  ";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 5);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 6);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeUses(root, {child, child2});
    std::vector<std::string> getUses = readManager.getUses(1);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(getUses, expected));

    ASTNode root3 = ASTNode(type, std::string("value"), 2);
    ASTNode child4 = ASTNode(type, std::string("tbate3"), 7);
    ASTNode child5 = ASTNode(type, std::string("tbate4"), 8);

    root3.addChild(std::make_shared<ASTNode>(child4));
    root3.addChild(std::make_shared<ASTNode>(child5));

    writeFacade.storeUses(root3, {child4, child5});
    REQUIRE(is_same_vector(getUses, expected));

    getUses = readManager.getUses(2);
    expected = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(getUses, expected));

    bool trueResult = readManager.getUses(1, "tbate");
    bool trueResult2 = readManager.getUses(2, "tbate3");
    REQUIRE((trueResult && trueResult2));

    bool falseResult = readManager.getUses(2, "tbate");
    bool falseResult2 = readManager.getUses(1, "tbate3");
    REQUIRE(!(falseResult && falseResult2));
}

TEST_CASE("Read Write modifiesARe") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "modifiesARe  ";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 5);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 6);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeModifies(root, {child, child2});
    std::vector<std::string> getModifies = readManager.getModifies(1);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(getModifies, expected));

    ASTNode root3 = ASTNode(type, std::string("value"), 2);
    ASTNode child4 = ASTNode(type, std::string("tbate3"), 7);
    ASTNode child5 = ASTNode(type, std::string("tbate4"), 8);

    root3.addChild(std::make_shared<ASTNode>(child4));
    root3.addChild(std::make_shared<ASTNode>(child5));

    writeFacade.storeModifies(root3, {child4, child5});
    REQUIRE(is_same_vector(getModifies, expected));

    getModifies = readManager.getModifies(2);
    expected = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(getModifies, expected));

    bool trueResult = readManager.getModifies(1, "tbate");
    bool trueResult2 = readManager.getModifies(2, "tbate3");
    REQUIRE((trueResult && trueResult2));

    bool falseResult = readManager.getModifies(2, "tbate");
    bool falseResult2 = readManager.getModifies(1, "tbate3");
    REQUIRE(!(falseResult && falseResult2));
}

TEST_CASE("Test StmtToProcName") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    writeFacade.storeCallStmtToProcName(1, "1");
    std::string first = readManager.getCallStmtToProcName(1);
    REQUIRE(first == "1");

    writeFacade.storeCallStmtToProcName(2, "2");
    std::string second = readManager.getCallStmtToProcName(2);
    REQUIRE(second == "2");

    writeFacade.storeCallStmtToProcName(3, "3");
    std::string third = readManager.getCallStmtToProcName(3);
    REQUIRE(third == "3");

    std::vector<int> temp = readManager.getAllCallStmtToProcName();
    REQUIRE(is_same_vector(temp, std::vector<int>{1, 2, 3}));
}

TEST_CASE("Test UsesProc") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<std::string> empty = readManager.getUsesProc("value");
    std::vector<std::string> expectedEmpty = {};
    REQUIRE(is_same_vector(empty, expectedEmpty));

    std::string type = "UsesProc  ";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 2);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 3);
    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeUsesProc(root, {child, child2});
    std::vector<std::string> first = readManager.getUsesProc("value");
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(first, expected));

    ASTNode root4 = ASTNode(type, std::string("value2"), 4);
    ASTNode child5 = ASTNode(type, std::string("tbate3"), 5);
    ASTNode child6 = ASTNode(type, std::string("tbate4"), 6);
    root4.addChild(std::make_shared<ASTNode>(child5));
    root4.addChild(std::make_shared<ASTNode>(child6));

    writeFacade.storeUsesProc(root4, {child5, child6});
    std::vector<std::string> second = readManager.getUsesProc("value2");
    std::vector<std::string> expected2 = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(second, expected2));
    REQUIRE(readManager.getUsesProc("value", "tbate"));
    REQUIRE(readManager.getUsesProc("value", "tbate2"));
    REQUIRE(readManager.getUsesProc("value2", "tbate3"));
    REQUIRE(readManager.getUsesProc("value2", "tbate4"));
}

TEST_CASE("Test UsesCall") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "UsesCall ";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 2);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 3);
    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeUses(root, {child, child2});
    std::vector<std::string> first = readManager.getUses(1);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(first, expected));

    ASTNode root4 = ASTNode(type, std::string("value2"), 4);
    ASTNode child5 = ASTNode(type, std::string("tbate3"), 5);
    ASTNode child6 = ASTNode(type, std::string("tbate4"), 6);
    root4.addChild(std::make_shared<ASTNode>(child5));
    root4.addChild(std::make_shared<ASTNode>(child6));

    writeFacade.storeUses(root4, {child5, child6});
    std::vector<std::string> second = readManager.getUses(4);
    std::vector<std::string> expected2 = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(second, expected2));
    REQUIRE(readManager.getUses(1, "tbate"));
    REQUIRE(readManager.getUses(1, "tbate2"));
    REQUIRE(readManager.getUses(4, "tbate3"));
    REQUIRE(readManager.getUses(4, "tbate4"));
}

TEST_CASE("Test ModifiesCall") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "ModifiesCall ";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 2);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 3);
    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeModifies(root, {child, child2});
    std::vector<std::string> first = readManager.getModifies(1);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(first, expected));

    ASTNode root4 = ASTNode(type, std::string("value2"), 4);
    ASTNode child5 = ASTNode(type, std::string("tbate3"), 5);
    ASTNode child6 = ASTNode(type, std::string("tbate4"), 6);
    root4.addChild(std::make_shared<ASTNode>(child5));
    root4.addChild(std::make_shared<ASTNode>(child6));

    writeFacade.storeModifies(root4, {child5, child6});
    std::vector<std::string> second = readManager.getModifies(4);
    std::vector<std::string> expected2 = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(second, expected2));
    REQUIRE(readManager.getModifies(1, "tbate"));
    REQUIRE(readManager.getModifies(1, "tbate2"));
    REQUIRE(readManager.getModifies(4, "tbate3"));
    REQUIRE(readManager.getModifies(4, "tbate4"));
}

TEST_CASE("Test ModifiesProc") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<std::string> empty = readManager.getModifiesProc("value");
    std::vector<std::string> expectedEmpty = {};
    REQUIRE(is_same_vector(empty, expectedEmpty));

    std::string type = "ModifiesProc  ";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 2);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 3);
    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeModifiesProc(root, {child, child2});
    std::vector<std::string> first = readManager.getModifiesProc("value");
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(first, expected));

    ASTNode root4 = ASTNode(type, std::string("value2"), 4);
    ASTNode child5 = ASTNode(type, std::string("tbate3"), 5);
    ASTNode child6 = ASTNode(type, std::string("tbate4"), 6);
    root4.addChild(std::make_shared<ASTNode>(child5));
    root4.addChild(std::make_shared<ASTNode>(child6));

    writeFacade.storeModifiesProc(root4, {child5, child6});
    std::vector<std::string> second = readManager.getModifiesProc("value2");
    std::vector<std::string> expected2 = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(second, expected2));
    REQUIRE(readManager.getModifiesProc("value", "tbate"));
    REQUIRE(readManager.getModifiesProc("value", "tbate2"));
    REQUIRE(readManager.getModifiesProc("value2", "tbate3"));
    REQUIRE(readManager.getModifiesProc("value2", "tbate4"));
}

TEST_CASE("Test Call") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<std::string> keys = readManager.getCallsS2("tbate2", false);
    REQUIRE(keys.size() == 0);

    readManager.clearCache();

    std::vector<std::string> empty = readManager.getCalls("value", false);
    std::vector<std::string> empty2 = readManager.getCallsS2("value", false);
    bool empty3 = readManager.hasCalls(false);
    REQUIRE(!empty3);
    std::vector<std::string> expectedEmpty = {};
    std::vector<std::vector<std::string>> expectedEmpty2 = {};
    REQUIRE(is_same_vector(empty, expectedEmpty));
    REQUIRE(is_same_vector(empty2, expectedEmpty));

    writeFacade.storeCalls("value", {"tbate", "tbate2"}, false);
    std::vector<std::string> first = readManager.getCalls("value", false);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(first, expected));

    writeFacade.storeCalls("tbate2", {"tbate3", "tbate4"}, false);
    std::vector<std::string> second = readManager.getCalls("tbate2", false);
    std::vector<std::string> expected2 = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(second, expected2));

    std::vector<std::string> firstS2 = readManager.getCallsS2("tbate2", false);
    std::vector<std::string> expectedS2 = {"value"};
    REQUIRE(is_same_vector(firstS2, expectedS2));

    std::vector<std::string> secondS2 = readManager.getCallsS2("tbate4", false);
    std::vector<std::string> secondexpectedS2 = {"tbate2"};
    REQUIRE(is_same_vector(firstS2, expectedS2));

    bool hasCalls = readManager.hasCalls(false);
    REQUIRE(hasCalls);
}

TEST_CASE("Test Call Star") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<std::string> keys = readManager.getCallsS2("tbate5", true);
    REQUIRE(keys.size() == 0);

    readManager.clearCache();

    std::vector<std::string> empty = readManager.getCalls("value", true);
    std::vector<std::string> empty2 = readManager.getCallsS2("value", true);
    bool hasCalls = readManager.hasCalls(true);
    REQUIRE(!hasCalls);
    std::vector<std::string> expectedEmpty = {};
    std::vector<std::vector<std::string>> expectedEmpty2 = {};
    REQUIRE(is_same_vector(empty, expectedEmpty));
    REQUIRE(is_same_vector(empty2, expectedEmpty));

    writeFacade.storeCalls("value", {"tbate", "tbate2", "tbate3", "tbate4", "tbate5", "tbate6"}, true);
    std::vector<std::string> first = readManager.getCalls("value", true);
    std::vector<std::string> expected = {
        "tbate",
        "tbate2",
        "tbate3",
        "tbate4",
        "tbate5",
        "tbate6",
    };
    REQUIRE(is_same_vector(first, expected));

    writeFacade.storeCalls("tbate2", {"tbate3", "tbate4", "tbate5", "tbate6"}, true);
    writeFacade.storeCalls("tbate4", {"tbate5", "tbate6"}, true);
    std::vector<std::string> second = readManager.getCalls("tbate2", true);
    std::vector<std::string> expected2 = {"tbate3", "tbate4", "tbate5", "tbate6"};
    REQUIRE(is_same_vector(second, expected2));

    std::vector<std::string> firstS2 = readManager.getCallsS2("tbate5", true);
    std::vector<std::string> expectedS2 = {
        "value",
        "tbate2",
        "tbate4",
    };
    REQUIRE(is_same_vector(firstS2, expectedS2));

    std::vector<std::string> secondS2 = readManager.getCallsS2("tbate3", true);
    std::vector<std::string> secondexpectedS2 = {"value", "tbate2"};
    REQUIRE(is_same_vector(firstS2, expectedS2));

    hasCalls = readManager.hasCalls(true);
    REQUIRE(hasCalls);
}

TEST_CASE("Read Write If Pattern Test") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    WriteFacade writeFacade(storageManager);
    ReadFacade readFacade(storageManager);

    std::string cacheCheck = "juice";
    std::vector<int> keys = readFacade.getIfPattern(cacheCheck);
    REQUIRE(keys.size() == 0);

    readFacade.clearCache();

    std::vector<std::string> ifVariables = {"x", "juice", "X", "k"};
    std::vector<std::string> ifVariables2 = {"x", "X"};
    writeFacade.storeIfPattern(ifVariables, 1);
    writeFacade.storeIfPattern(ifVariables2, 3);

    SECTION("get if pattern matching - with results") {
        std::string x = "x";
        std::string X = "X";
        std::string juice = "juice";

        std::vector<int> xValues = readFacade.getIfPattern(x);
        std::vector<int> x_Values = readFacade.getIfPattern(X);
        std::vector<int> juiceValues = readFacade.getIfPattern(juice);
        REQUIRE(is_same_vector(xValues, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(x_Values, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(juiceValues, std::vector<int>{1}));
    }

    SECTION("get if pattern matching - no results") {
        std::string nonexistent = "nonexistent";
        std::string emptyString;

        std::vector<int> xValues = readFacade.getIfPattern(nonexistent);
        std::vector<int> x_Values = readFacade.getIfPattern(emptyString);
        REQUIRE(xValues.empty());
        REQUIRE(x_Values.empty());
    }
}

TEST_CASE("Read Write While Pattern Test") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    WriteFacade writeFacade(storageManager);
    ReadFacade readFacade(storageManager);

    std::string cacheCheck = "juice";
    std::vector<int> keys = readFacade.getWhilePattern(cacheCheck);
    REQUIRE(keys.size() == 0);

    readFacade.clearCache();

    std::vector<std::string> var = {"x", "juice", "X", "k"};
    std::vector<std::string> var2 = {"x", "X"};
    writeFacade.storeWhilePattern(var, 1);
    writeFacade.storeWhilePattern(var2, 3);

    SECTION("get if pattern matching - with results") {
        std::string x = "x";
        std::string X = "X";
        std::string juice = "juice";

        std::vector<int> xValues = readFacade.getWhilePattern(x);
        std::vector<int> x_Values = readFacade.getWhilePattern(X);
        std::vector<int> juiceValues = readFacade.getWhilePattern(juice);
        REQUIRE(is_same_vector(xValues, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(x_Values, std::vector<int>{3, 1}));
        REQUIRE(is_same_vector(juiceValues, std::vector<int>{1}));
    }

    SECTION("get if pattern matching - no results") {
        std::string nonexistent = "nonexistent";
        std::string emptyString;

        std::vector<int> xValues = readFacade.getWhilePattern(nonexistent);
        std::vector<int> x_Values = readFacade.getWhilePattern(emptyString);
        REQUIRE(xValues.empty());
        REQUIRE(x_Values.empty());
    }
}

TEST_CASE("Test ReadToVarName") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string empty = readManager.getReadVarName(1);
    REQUIRE(empty == std::string());

    std::string s1 = std::string("1");
    std::string s2 = std::string("2");
    std::string s3 = std::string("3");
    writeFacade.storeReadStmt(1, s1);
    std::string first = readManager.getReadVarName(1);
    REQUIRE(first == "1");

    writeFacade.storeReadStmt(2, s2);
    std::string second = readManager.getReadVarName(2);
    REQUIRE(second == "2");

    writeFacade.storeReadStmt(3, s3);
    std::string third = readManager.getReadVarName(3);
    REQUIRE(third == "3");
}

TEST_CASE("Test PrintToVarName") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string empty = readManager.getPrintVarName(1);
    REQUIRE(empty == std::string());

    std::string s1 = std::string("1");
    std::string s2 = std::string("2");
    std::string s3 = std::string("3");
    writeFacade.storePrintStmt(1, s1);
    std::string first = readManager.getPrintVarName(1);
    REQUIRE(first == "1");

    writeFacade.storePrintStmt(2, s2);
    std::string second = readManager.getPrintVarName(2);
    REQUIRE(second == "2");

    writeFacade.storePrintStmt(3, s3);
    std::string third = readManager.getPrintVarName(3);
    REQUIRE(third == "3");
}

TEST_CASE("Read Write uses") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "uses";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 5);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 6);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeUses(root, {child, child2});
    std::vector<std::string> getUses = readManager.getUses(1);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(getUses, expected));

    ASTNode root3 = ASTNode(type, std::string("value"), 2);
    ASTNode child4 = ASTNode(type, std::string("tbate3"), 7);
    ASTNode child5 = ASTNode(type, std::string("tbate4"), 8);

    root3.addChild(std::make_shared<ASTNode>(child4));
    root3.addChild(std::make_shared<ASTNode>(child5));

    writeFacade.storeUses(root3, {child4, child5});
    REQUIRE(is_same_vector(getUses, expected));

    getUses = readManager.getUses(2);
    expected = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(getUses, expected));

    bool trueResult = readManager.getUses(1, "tbate");
    bool trueResult2 = readManager.getUses(2, "tbate3");
    REQUIRE((trueResult && trueResult2));

    bool falseResult = readManager.getUses(2, "tbate");
    bool falseResult2 = readManager.getUses(1, "tbate3");
    REQUIRE(!(falseResult && falseResult2));
}

TEST_CASE("Read Write modifies") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::string type = "modifies";
    ASTNode root = ASTNode(type, std::string("value"), 1);
    ASTNode child = ASTNode(type, std::string("tbate"), 5);
    ASTNode child2 = ASTNode(type, std::string("tbate2"), 6);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeModifies(root, {child, child2});
    std::vector<std::string> getModifies = readManager.getModifies(1);
    std::vector<std::string> expected = {"tbate", "tbate2"};
    REQUIRE(is_same_vector(getModifies, expected));

    ASTNode root3 = ASTNode(type, std::string("value"), 2);
    ASTNode child4 = ASTNode(type, std::string("tbate3"), 7);
    ASTNode child5 = ASTNode(type, std::string("tbate4"), 8);

    root3.addChild(std::make_shared<ASTNode>(child4));
    root3.addChild(std::make_shared<ASTNode>(child5));

    writeFacade.storeModifies(root3, {child4, child5});
    REQUIRE(is_same_vector(getModifies, expected));

    getModifies = readManager.getModifies(2);
    expected = {"tbate3", "tbate4"};
    REQUIRE(is_same_vector(getModifies, expected));

    bool trueResult = readManager.getModifies(1, "tbate");
    bool trueResult2 = readManager.getModifies(2, "tbate3");
    REQUIRE((trueResult && trueResult2));

    bool falseResult = readManager.getModifies(2, "tbate");
    bool falseResult2 = readManager.getModifies(1, "tbate3");
    REQUIRE(!(falseResult && falseResult2));
}

TEST_CASE("Read Write Parents Bool") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<int> empty = {};
    std::vector<int> getParent = readManager.getParents(1, true);
    REQUIRE(getParent == empty);
    std::vector<int> getParentStar = readManager.getParents(1, false);
    REQUIRE(getParentStar == empty);

    std::string type = "assign";
    std::string value = "x";
    ASTNode root = ASTNode(type, value, 1);
    ASTNode child = ASTNode(type, value, 2);
    ASTNode child2 = ASTNode(type, value, 3);

    root.addChild(std::make_shared<ASTNode>(child));
    root.addChild(std::make_shared<ASTNode>(child2));

    writeFacade.storeParents(root, {child, child2}, true);
    getParent = readManager.getParents(1, true);
    std::vector<int> expected = {2, 3};
    REQUIRE(getParent == expected);

    writeFacade.storeParents(root, {child}, false);
    getParentStar = readManager.getParents(1, false);
    expected = {2};
    REQUIRE(getParentStar == expected);
}

TEST_CASE("Read Write next Bool") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<int> empty = {};
    std::vector<int> getNext = readManager.getNext(1, false);
    REQUIRE(is_same_vector(empty, getNext));
    CFGNode c1 = createCFGNode(1, std::vector<int>{2, 3});
    CFGNode c2 = createCFGNode(2, std::vector<int>{4});
    CFGNode c3 = createCFGNode(3, std::vector<int>{4});
    CFGNode c4 = createCFGNode(4, std::vector<int>{5});
    CFGNode c5 = createCFGNode(5, std::vector<int>{});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);
    writeFacade.storeNext(c5);

    getNext = readManager.getNext(1, false);
    REQUIRE(is_same_vector(getNext, std::vector<int>{2, 3}));
    getNext = readManager.getNext(2, false);
    REQUIRE(is_same_vector(getNext, std::vector<int>{4}));
    getNext = readManager.getNext(3, false);
    REQUIRE(is_same_vector(getNext, std::vector<int>{4}));
    getNext = readManager.getNext(4, false);
    REQUIRE(is_same_vector(getNext, std::vector<int>{5}));
}

TEST_CASE("Read Write nextstar Bool") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2, 3});
    CFGNode c2 = createCFGNode(2, std::vector<int>{4});
    CFGNode c3 = createCFGNode(3, std::vector<int>{4});
    CFGNode c4 = createCFGNode(4, std::vector<int>{5});

    REQUIRE(!readManager.hasNext());

    bool hasNextStar = readManager.hasNextStar();
    REQUIRE(!hasNextStar);

    writeFacade.storeNext(c1);

    hasNextStar = readManager.hasNextStar();
    REQUIRE(!hasNextStar);

    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);

    REQUIRE(readManager.hasNext());

    hasNextStar = readManager.hasNextStar();
    REQUIRE(hasNextStar);

    std::vector<int> getNext2 = readManager.getNext(1, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{2, 3, 4, 5}));
    getNext2 = readManager.getNext(2, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{4, 5}));
    getNext2 = readManager.getNext(2, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{4, 5}));  // test cache
    getNext2 = readManager.getNext(3, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{4, 5}));
    getNext2 = readManager.getNext(4, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{5}));
    getNext2 = readManager.getNext(5, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{}));
}

TEST_CASE("Read Write nextstar cyclic") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2});
    CFGNode c2 = createCFGNode(2, std::vector<int>{3});
    CFGNode c3 = createCFGNode(3, std::vector<int>{4});
    CFGNode c4 = createCFGNode(4, std::vector<int>{1});

    REQUIRE(!readManager.hasNext());

    bool hasNextStar = readManager.hasNextStar();
    REQUIRE(!hasNextStar);

    writeFacade.storeNext(c1);

    hasNextStar = readManager.hasNextStar();
    REQUIRE(!hasNextStar);

    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);

    REQUIRE(readManager.hasNext());

    hasNextStar = readManager.hasNextStar();
    REQUIRE(hasNextStar);

    std::vector<int> getNext2 = readManager.getNext(1, true);
    CAPTURE(getNext2);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{2, 3, 4, 1}));
    getNext2 = readManager.getNext(2, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{2, 3, 4, 1}));
    getNext2 = readManager.getNext(2, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{2, 3, 4, 1}));  // test cache
    getNext2 = readManager.getNext(3, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{2, 3, 4, 1}));
    getNext2 = readManager.getNext(4, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{2, 3, 4, 1}));
}

TEST_CASE("Read Write nextstar cyclic2") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c27 = createCFGNode(27, std::vector<int>{28, 29});
    CFGNode c28 = createCFGNode(28, std::vector<int>{27});
    CFGNode c29 = createCFGNode(29, std::vector<int>{30});
    CFGNode c30 = createCFGNode(30, std::vector<int>{31});

    REQUIRE(!readManager.hasNext());

    bool hasNextStar = readManager.hasNextStar();
    REQUIRE(!hasNextStar);

    writeFacade.storeNext(c27);

    hasNextStar = readManager.hasNextStar();
    REQUIRE(!hasNextStar);

    writeFacade.storeNext(c28);
    writeFacade.storeNext(c29);
    writeFacade.storeNext(c30);

    REQUIRE(readManager.hasNext());

    hasNextStar = readManager.hasNextStar();
    REQUIRE(hasNextStar);

    std::vector<int> getNext2 = readManager.getNext(27, true);
    CAPTURE(getNext2);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{27, 28, 29, 30, 31}));
    getNext2 = readManager.getNext(27, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{27, 28, 29, 30, 31}));
}

TEST_CASE("Read Write next graph big") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    std::vector<int> empty = {};
    std::vector<int> getNext = readManager.getNext(1, false);
    REQUIRE(is_same_vector(empty, getNext));
    CFGNode c1 = createCFGNode(1, std::vector<int>{2});
    CFGNode c2 = createCFGNode(2, std::vector<int>{3, 10});
    CFGNode c3 = createCFGNode(3, std::vector<int>{4});
    CFGNode c4 = createCFGNode(4, std::vector<int>{5, 9});
    CFGNode c5 = createCFGNode(5, std::vector<int>{6});
    CFGNode c6 = createCFGNode(6, std::vector<int>{7});
    CFGNode c7 = createCFGNode(7, std::vector<int>{8});
    CFGNode c8 = createCFGNode(8, std::vector<int>{4});
    CFGNode c9 = createCFGNode(9, std::vector<int>{1});
    CFGNode c10 = createCFGNode(10, std::vector<int>{11, 12});
    CFGNode c11 = createCFGNode(11, std::vector<int>{10});
    CFGNode c12 = createCFGNode(12, std::vector<int>{13, 15});
    CFGNode c13 = createCFGNode(13, std::vector<int>{14});
    CFGNode c14 = createCFGNode(14, std::vector<int>{17});
    CFGNode c15 = createCFGNode(15, std::vector<int>{16});
    CFGNode c16 = createCFGNode(16, std::vector<int>{17});
    CFGNode c17 = createCFGNode(17, std::vector<int>{18});
    CFGNode c18 = createCFGNode(18, std::vector<int>{19});
    CFGNode c19 = createCFGNode(19, std::vector<int>{1});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);
    writeFacade.storeNext(c5);
    writeFacade.storeNext(c6);
    writeFacade.storeNext(c7);
    writeFacade.storeNext(c8);
    writeFacade.storeNext(c9);
    writeFacade.storeNext(c10);
    writeFacade.storeNext(c11);
    writeFacade.storeNext(c12);
    writeFacade.storeNext(c13);
    writeFacade.storeNext(c14);
    writeFacade.storeNext(c15);
    writeFacade.storeNext(c16);
    writeFacade.storeNext(c17);
    writeFacade.storeNext(c18);
    writeFacade.storeNext(c19);
    getNext = readManager.getNextS2(10, false);
    CAPTURE(getNext);
    REQUIRE(is_same_vector(getNext, std::vector<int>{2, 11}));
    getNext = readManager.getNextS2(10, false);
    CAPTURE(getNext);
    REQUIRE(is_same_vector(getNext, std::vector<int>{2, 11}));
}

TEST_CASE("Read Write nextstarS2 Bool") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2, 3});
    CFGNode c2 = createCFGNode(2, std::vector<int>{4});
    CFGNode c3 = createCFGNode(3, std::vector<int>{4});
    CFGNode c4 = createCFGNode(4, std::vector<int>{5});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);

    std::vector<int> getNext2 = readManager.getNextS2(5, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{1, 2, 3, 4}));
    getNext2 = readManager.getNextS2(4, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{1, 2, 3}));
    getNext2 = readManager.getNextS2(3, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{1}));
    getNext2 = readManager.getNextS2(2, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{1}));
    getNext2 = readManager.getNextS2(1, true);
    REQUIRE(is_same_vector(getNext2, std::vector<int>{}));
}

TEST_CASE("Read Write getAffects simple") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2, 3});
    CFGNode c2 = createCFGNode(2, std::vector<int>{4});
    CFGNode c3 = createCFGNode(3, std::vector<int>{4});
    CFGNode c4 = createCFGNode(4, std::vector<int>{5});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);

    writeFacade.storeModifies(createASTNodes(1), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeModifies(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));

    writeFacade.storeUses(createASTNodes(2), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeUses(createASTNodes(3), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeUses(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeAssign(1);
    writeFacade.storeAssign(2);
    writeFacade.storeAssign(3);
    writeFacade.storeAssign(4);
    writeFacade.storeAssign(5);
    writeFacade.storeAssign(20);

    SECTION("getAffects arg1") {
        std::vector<int> res1 = readManager.getAffects(1);
        std::vector<int> out1 = std::vector<int>{2, 3, 4};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects arg1 no result") {
        std::vector<int> res1 = readManager.getAffects(2);
        REQUIRE(res1.empty());
    }

    SECTION("getAffects arg1 - modified v1 in stmt 3") {
        writeFacade.storeModifies(createASTNodes(3), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res2 = readManager.getAffects(1);
        std::vector<int> out2 = std::vector<int>{2, 3, 4};
        std::sort(res2.begin(), res2.end());
        REQUIRE(res2 == out2);
    }

    SECTION("getAffects arg2") {
        writeFacade.storeUses(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res1 = readManager.getAffectsS2(5);
        std::vector<int> out1 = std::vector<int>{1};
        REQUIRE(res1 == out1);
    }
    SECTION("getAffects arg2 no result") {
        std::vector<int> res1 = readManager.getAffectsS2(20);
        REQUIRE(res1.empty());
    }

    SECTION("getAffects arg2 - modified v1 in stmt 3") {
        writeFacade.storeUses(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeModifies(createASTNodes(3), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res2 = readManager.getAffectsS2(5);
        std::vector<int> out2 = std::vector<int>{1, 3};
        std::sort(res2.begin(), res2.end());
        REQUIRE(res2 == out2);
    }
}

TEST_CASE("Read Write getAffects cyclic") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2});
    CFGNode c2 = createCFGNode(2, std::vector<int>{3});
    CFGNode c3 = createCFGNode(3, std::vector<int>{5, 4, 2});
    CFGNode c4 = createCFGNode(4, std::vector<int>{6});
    CFGNode c5 = createCFGNode(5, std::vector<int>{6});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);
    writeFacade.storeNext(c5);

    writeFacade.storeModifies(createASTNodes(1), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));

    writeFacade.storeUses(createASTNodes(3), createASTNodeFromStringVector(std::vector<std::string>{"v1", "v2", "v3"}));
    writeFacade.storeUses(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeUses(createASTNodes(6), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeAssign(1);
    writeFacade.storeAssign(2);
    writeFacade.storeAssign(3);
    writeFacade.storeAssign(5);
    writeFacade.storeAssign(6);
    writeFacade.storeAssign(4);

    SECTION("getAffects arg1 cyclic not stuck") {
        std::vector<int> res1 = readManager.getAffects(1);
        std::vector<int> out1 = std::vector<int>{3, 4, 6};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects arg1 - alternate path exist") {
        writeFacade.storeModifies(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res2 = readManager.getAffects(1);
        std::vector<int> out2 = std::vector<int>{3, 4, 6};
        std::sort(res2.begin(), res2.end());
        REQUIRE(res2 == out2);
    }

    SECTION("getAffects arg1 - no alternate path exist") {
        writeFacade.storeModifies(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeModifies(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res2 = readManager.getAffects(1);
        std::vector<int> out2 = std::vector<int>{3, 4};
        std::sort(res2.begin(), res2.end());
        REQUIRE(res2 == out2);
    }

    SECTION("getAffects arg2 cyclic not stuck") {
        std::vector<int> res1 = readManager.getAffectsS2(4);
        std::vector<int> res2 = readManager.getAffectsS2(6);

        std::vector<int> out1 = std::vector<int>{1};
        REQUIRE(res1 == out1);
        REQUIRE(res2 == out1);
    }

    SECTION("getAffects arg2 - alternate path exist") {
        writeFacade.storeModifies(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res2 = readManager.getAffectsS2(6);
        std::vector<int> out2 = std::vector<int>{1, 5};
        std::sort(res2.begin(), res2.end());
        REQUIRE(res2 == out2);
    }

    SECTION("getAffects arg2 - more alternate path exist") {
        writeFacade.storeModifies(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeModifies(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeModifies(createASTNodes(2), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res2 = readManager.getAffectsS2(6);
        std::vector<int> out2 = std::vector<int>{4, 5};
        std::sort(res2.begin(), res2.end());
        REQUIRE(res2 == out2);
    }

    SECTION("getAffects arg2 - many var used") {
        std::vector<std::string> manyVar = std::vector<std::string>{"v1", "v2", "v3", "v4", "v5", "v6"};
        writeFacade.storeUses(createASTNodes(6), createASTNodeFromStringVector(manyVar));
        writeFacade.storeModifies(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v2", "v6", "v3"}));
        writeFacade.storeModifies(createASTNodes(2), createASTNodeFromStringVector(std::vector<std::string>{"v5", "v6"}));

        std::vector<int> res1 = readManager.getAffectsS2(6);
        std::vector<int> out1 = std::vector<int>{1, 2, 5};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }
}

TEST_CASE("Read Write getAffects complex") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2, 3});
    CFGNode c2 = createCFGNode(2, std::vector<int>{4, 5});
    CFGNode c3 = createCFGNode(3, std::vector<int>{7, 6});
    CFGNode c4 = createCFGNode(4, std::vector<int>{8});
    CFGNode c5 = createCFGNode(5, std::vector<int>{8});
    CFGNode c6 = createCFGNode(6, std::vector<int>{9});
    CFGNode c7 = createCFGNode(7, std::vector<int>{9});
    CFGNode c8 = createCFGNode(8, std::vector<int>{10});
    CFGNode c9 = createCFGNode(9, std::vector<int>{10});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);
    writeFacade.storeNext(c5);
    writeFacade.storeNext(c6);
    writeFacade.storeNext(c7);
    writeFacade.storeNext(c8);
    writeFacade.storeNext(c9);

    writeFacade.storeModifies(createASTNodes(1), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeModifies(createASTNodes(6), createASTNodeFromStringVector(std::vector<std::string>{"v5"}));
    writeFacade.storeModifies(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));

    writeFacade.storeUses(createASTNodes(1), createASTNodeFromStringVector(std::vector<std::string>{"v1", "v2", "v3"}));
    writeFacade.storeUses(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
    writeFacade.storeUses(createASTNodes(10), createASTNodeFromStringVector(std::vector<std::string>{"v1", "v3", "v5"}));
    writeFacade.storeAssign(1);
    writeFacade.storeAssign(2);
    writeFacade.storeAssign(3);
    writeFacade.storeAssign(4);
    writeFacade.storeAssign(5);
    writeFacade.storeAssign(6);
    writeFacade.storeAssign(6);
    writeFacade.storeAssign(7);
    writeFacade.storeAssign(8);
    writeFacade.storeAssign(10);

    SECTION("getAffects with results") {
        std::vector<int> res1 = readManager.getAffects(1);
        std::vector<int> out1 = std::vector<int>{4, 10};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects with no results") {
        writeFacade.storeModifies(createASTNodes(8), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeAssign(9);
        writeFacade.storeModifies(createASTNodes(9), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeModifies(createASTNodes(10), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res1 = readManager.getAffects(1);
        std::vector<int> out1 = std::vector<int>{4};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects arg2 with results") {
        std::vector<int> res1 = readManager.getAffectsS2(10);
        std::vector<int> out1 = std::vector<int>{1, 4, 6};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects call procName modifies variable, no path") {
        writeFacade.storeModifies(createASTNodes(8), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeCalls("proc1", std::vector<std::string>{"proc2"}, false);
        writeFacade.storeCallStmtToProcName(9, "proc2");
        writeFacade.storeModifiesProc(createASTNodes("proc2"), createASTNodeFromStringVector(std::vector<std::string>{"v10", "v1", "v2"}));
        std::vector<int> res1 = readManager.getAffects(1);
        std::vector<int> out1 = std::vector<int>{4};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects call procName modifies variable, no path exist") {
        writeFacade.storeCalls("proc1", std::vector<std::string>{"proc2"}, false);
        writeFacade.storeCallStmtToProcName(9, "proc2");
        writeFacade.storeModifiesProc(createASTNodes("proc2"), createASTNodeFromStringVector(std::vector<std::string>{"v10", "v1", "v2"}));
        writeFacade.storeModifies(createASTNodes(2), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        std::vector<int> res1 = readManager.getAffects(1);
        std::vector<int> out1 = std::vector<int>{};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }

    SECTION("getAffects arg2 call procName modifies variable, path exist") {
        writeFacade.storeCalls("proc1", std::vector<std::string>{"proc2"}, false);
        writeFacade.storeCallStmtToProcName(9, "proc2");
        writeFacade.storeModifiesProc(createASTNodes("proc2"), createASTNodeFromStringVector(std::vector<std::string>{"v10", "v1", "v2"}));
        std::vector<int> res1 = readManager.getAffectsS2(10);
        std::vector<int> out1 = std::vector<int>{1, 4, 6};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
    }
}

TEST_CASE("Read Write hasAffects") {
    // Create instance storage and wrap them in shared pointers
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    const ReadFacade readManager(storageManager);
    WriteFacade writeFacade(storageManager);

    CFGNode c1 = createCFGNode(1, std::vector<int>{2, 3});
    CFGNode c2 = createCFGNode(2, std::vector<int>{4});
    CFGNode c3 = createCFGNode(3, std::vector<int>{5});
    CFGNode c4 = createCFGNode(4, std::vector<int>{5});

    writeFacade.storeNext(c1);
    writeFacade.storeNext(c2);
    writeFacade.storeNext(c3);
    writeFacade.storeNext(c4);

    SECTION("no assign stmts") {
        std::vector<int> res1 = readManager.getAffects(1);
        bool res2 = readManager.hasAffects();
        std::vector<int> out1 = std::vector<int>{};
        std::sort(res1.begin(), res1.end());
        REQUIRE(res1 == out1);
        REQUIRE_FALSE(res2);
    }

    SECTION("hasAffects returns true") {
        writeFacade.storeAssign(1);
        writeFacade.storeAssign(4);
        writeFacade.storeAssign(2);
        writeFacade.storeAssign(3);

        writeFacade.storeModifies(createASTNodes(2), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeModifies(createASTNodes(3), createASTNodeFromStringVector(std::vector<std::string>{"v2"}));
        writeFacade.storeModifies(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v2"}));

        writeFacade.storeUses(createASTNodes(4), createASTNodeFromStringVector(std::vector<std::string>{"v1"}));
        writeFacade.storeUses(createASTNodes(5), createASTNodeFromStringVector(std::vector<std::string>{"v1", "v2", "v5"}));

        bool affected = readManager.hasAffects();
        REQUIRE(affected);
    }

    SECTION("hasAffects on call or read stmt") {
        std::string test = "test";
        writeFacade.storeReadStmt(1, test);
        writeFacade.storeCallStmtToProcName(2, test);
        writeFacade.storeAssign(1);

        bool affected = readManager.hasAffects();
        REQUIRE_FALSE(affected);
    }
}