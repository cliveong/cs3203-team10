#include "../../spa/src/pkb/ReadFacade.h"
#include "../../spa/src/qps/QueryManager.h"
#include "catch.hpp"
using namespace std;

bool is_same_vector(std::vector<std::string> v1, std::vector<std::string> v2) {
    return std::is_permutation(v1.begin(), v1.end(), v2.begin());
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

TEST_CASE("Get All Variables Test") {
    std::shared_ptr<Storage> storageManager = make_shared<Storage>();
    std::string str1 = "x";
    std::string str2 = "y";
    std::string str3 = "z";
    storageManager->storeVariable(str1);
    storageManager->storeVariable(str2);
    storageManager->storeVariable(str3);

    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);

    QueryManager queryManager(readFacade);

    std::string query = "variable v; Select v";
    std::vector<std::string> actualResults = queryManager.process(query);

    std::vector<std::string> expectedResults = {"x", "y", "z"};

    REQUIRE(is_same_vector(actualResults, expectedResults));
}

TEST_CASE("Empty Variable Storage Test") {
    std::shared_ptr<Storage> storageManager = make_shared<Storage>();

    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);

    QueryManager manager = QueryManager(readFacade);
    std::string queryString = "variable v;\nSelect v";
    std::vector<std::string> actualResults = manager.process(queryString);

    std::vector<std::string> expectedResults = {};
    REQUIRE(is_same_vector(actualResults, expectedResults));
}

TEST_CASE("Get Follows Relationship Test") {
    std::shared_ptr<Storage> storageManager = make_shared<Storage>();
    storageManager->storeFollows(createASTNode(1), createASTNodesFromIntVector({2}));
    storageManager->storeFollows(createASTNode(2), createASTNodesFromIntVector({3}));
    storageManager->storeFollows(createASTNode(3), createASTNodesFromIntVector({4}));

    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);

    QueryManager manager = QueryManager(readFacade);
    std::string queryString = "stmt s, s1; Select s1 such that Follows(s, s1)";
    std::vector<std::string> actualResults = manager.process(queryString);

    std::vector<std::string> expectedResults = {"2", "3", "4"};

    REQUIRE(is_same_vector(actualResults, expectedResults));
}

TEST_CASE("Get Parent Relationship Test") {
    std::shared_ptr<Storage> storageManager = make_shared<Storage>();

    storageManager->storeAssign(1);
    storageManager->storeIf(2);

    std::vector<int> childStmts{3, 4, 5};
    storageManager->storeParents(createASTNode(2), createASTNodesFromIntVector(childStmts));

    storageManager->storeStmt(3);
    storageManager->storeStmt(4);
    storageManager->storeStmt(5);

    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);

    QueryManager manager = QueryManager(readFacade);
    std::string queryString = "if if; stmt s; Select s such that Parent(if, s)";
    std::vector<std::string> actualResults = manager.process(queryString);

    std::vector<std::string> expectedResults = {"3", "4", "5"};

    REQUIRE(is_same_vector(actualResults, expectedResults));
}
