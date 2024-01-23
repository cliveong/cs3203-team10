#include "catch.hpp"
#include "pkb/entity_managers/EntityManager.h"

TEST_CASE("EntityManager insertEntity Test with int") {
    EntityManager<int> entityManager;

    entityManager.insertEntity(1);
    int size = entityManager.getAllEntitiesEntries().size();

    REQUIRE(size == 1);

    // failed to insert due to duplicate
    entityManager.insertEntity(1);
    REQUIRE_FALSE(size == 2);
}

TEST_CASE("EntityManager insertEntity Test with string") {
    EntityManager<std::string> entityManager;

    const std::string testString = "test";

    entityManager.insertEntity(testString);
    int size = entityManager.getAllEntitiesEntries().size();

    REQUIRE(size == 1);

    // failed to insert due to duplicate
    entityManager.insertEntity(testString);
    REQUIRE_FALSE(size == 2);
}

TEST_CASE("EntityManager isEmpty Test") {
    EntityManager<int> entityManager;
    REQUIRE(entityManager.isEmpty());

    entityManager.insertEntity(1);

    REQUIRE_FALSE(entityManager.isEmpty());
}

TEST_CASE("EntityManager getAllEntitiesEntries Test") {
    EntityManager<int> entityManager;
    entityManager.insertEntity(2);
    entityManager.insertEntity(1);
    entityManager.insertEntity(3);

    std::unordered_set<int> allEntities = entityManager.getAllEntitiesEntries();

    REQUIRE(allEntities.find(1) != allEntities.end());
    REQUIRE(allEntities.find(2) != allEntities.end());
    REQUIRE(allEntities.find(3) != allEntities.end());
}
