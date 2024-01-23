#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "TestUtils.h"
#include "catch.hpp"
#include "qps/ResultsTable.h"

using namespace std;

TEST_CASE("Retrieve invalid synonym") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::string synonym = "v";
    std::vector<std::string> result = {"A", "B", "C"};
    resultTable->addSingleResult(synonym, result);
    std::vector<std::string> output = resultTable->getResult("w");

    REQUIRE(TestUtils::is_same_vector(output, {}));
}

TEST_CASE("Add Single Synonym and Result Test") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::string synonym = "v";
    std::vector<std::string> result = {"A", "B", "C"};
    resultTable->addSingleResult(synonym, result);
    std::vector<std::string> output = resultTable->getResult(synonym);

    REQUIRE(TestUtils::is_same_vector(output, result));
}

TEST_CASE("Add 2 different Synonym and Result Test") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::string synonym = "v";
    std::vector<std::string> result = {"A", "B", "C"};
    std::string synonym2 = "a";
    std::vector<std::string> result2 = {"1", "2", "3"};
    resultTable->addSingleResult(synonym, result);
    resultTable->addSingleResult(synonym2, result2);
    std::vector<std::string> output = resultTable->getResult(synonym);
    std::vector<std::string> output2 = resultTable->getResult(synonym2);

    REQUIRE(TestUtils::is_same_vector(output, result));
    REQUIRE(TestUtils::is_same_vector(output2, result2));
}

TEST_CASE("Add 2 same Synonym and Result Test") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::string synonym = "v";
    std::vector<std::string> result = {"A", "B", "C"};
    std::string synonym2 = "v";
    std::vector<std::string> result2 = {"B", "C", "D"};
    resultTable->addSingleResult(synonym, result);
    resultTable->addSingleResult(synonym2, result2);
    std::vector<std::string> output = resultTable->getResult(synonym);
    std::vector<std::string> output2 = resultTable->getResult(synonym2);
    std::vector<std::string> finalOutput = {"B", "C"};

    REQUIRE(TestUtils::is_same_vector(output, finalOutput));
    REQUIRE(TestUtils::is_same_vector(output2, finalOutput));
    REQUIRE(TestUtils::is_same_vector(output, output2));
}

TEST_CASE("Add pair results test") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::vector<std::string> synonyms = {"a", "v"};
    std::vector<std::vector<std::string>> resultList = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    resultTable->addPairResult(synonyms, resultList);
    std::vector<std::string> output = resultTable->getResult("a");
    std::vector<std::string> output2 = resultTable->getResult("v");
    std::vector<std::string> result = {"1", "2", "3"};
    std::vector<std::string> result2 = {"A", "B", "C"};

    REQUIRE(TestUtils::is_same_vector(output, result));
    REQUIRE(TestUtils::is_same_vector(output2, result2));
}

TEST_CASE("Add single, add pair results test") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::vector<std::string> synonyms = {"a", "v"};
    std::vector<std::vector<std::string>> resultList = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    resultTable->addSingleResult("v", {"A", "B"});
    resultTable->addPairResult(synonyms, resultList);
    std::vector<std::string> output = resultTable->getResult("a");
    std::vector<std::string> output2 = resultTable->getResult("v");
    std::vector<std::string> result = {"1", "2"};
    std::vector<std::string> result2 = {"A", "B"};

    REQUIRE(TestUtils::is_same_vector(output, result));
    REQUIRE(TestUtils::is_same_vector(output2, result2));
}

TEST_CASE("Add pairs with one common synonym results test") {
    std::shared_ptr<ResultsTable> resultTable = std::make_shared<ResultsTable>();
    std::vector<std::string> synonyms = {"a", "v"};
    std::vector<std::vector<std::string>> resultList = {{"1", "A"}, {"2", "B"}, {"3", "C"}};
    std::vector<std::string> synonyms2 = {"a", "w"};
    std::vector<std::vector<std::string>> resultList2 = {{"1", "5"}, {"2", "5"}, {"4", "5"}};
    resultTable->addPairResult(synonyms, resultList);
    resultTable->addPairResult(synonyms2, resultList2);
    std::vector<std::string> output = resultTable->getResult("a");
    std::vector<std::string> output2 = resultTable->getResult("v");
    std::vector<std::string> output3 = resultTable->getResult("w");
    std::vector<std::string> result = {"1", "2"};
    std::vector<std::string> result2 = {"A", "B"};
    std::vector<std::string> result3 = {"5"};

    REQUIRE(TestUtils::is_same_vector(output, result));
    REQUIRE(TestUtils::is_same_vector(output2, result2));
    REQUIRE(TestUtils::is_same_vector(output3, result3));
}
