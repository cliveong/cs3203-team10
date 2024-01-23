#include "catch.hpp"
#include "pkb/cfg/Affects.h"

TEST_CASE("Affects Test - insertAffects and clear cache") {
    Affects affectsCache;

    SECTION("Insert into cache") {
        affectsCache.insertAffects(1, std::vector<int>{2}, false);
        affectsCache.insertAffects(6, std::vector<int>{3, 4}, false);

        REQUIRE(affectsCache.getCache().count(1) == 1);
        REQUIRE(affectsCache.getCache().count(5) == 0);
        REQUIRE(affectsCache.getCache().count(6) == 1);
    }

    SECTION("Insert into reverse cache") {
        affectsCache.insertAffects(1, std::vector<int>{2}, true);
        affectsCache.insertAffects(3, std::vector<int>{3, 4}, true);

        REQUIRE(affectsCache.getReverseCache().count(1) == 1);
        REQUIRE(affectsCache.getReverseCache().count(30) == 0);
    }
}

TEST_CASE("Affects Test - clearCache") {
    Affects affectsCache;
    affectsCache.insertAffects(1, std::vector<int>{2}, false);
    affectsCache.insertAffects(6, std::vector<int>{3, 4}, true);
    REQUIRE_FALSE(affectsCache.getCache().empty());

    affectsCache.clearCache();
    REQUIRE(affectsCache.getCache().empty());
    REQUIRE(affectsCache.getReverseCache().empty());
}

TEST_CASE("Affects Test - getAffectedStmt") {
    Affects affectsCache;

    SECTION("get from cache") {
        affectsCache.insertAffects(1, std::vector<int>{2}, false);
        affectsCache.insertAffects(6, std::vector<int>{3, 4}, false);

        REQUIRE(affectsCache.getAffectedStmt(1, false).size() == 1);
        REQUIRE(affectsCache.getAffectedStmt(6, false).size() == 2);
    }

    SECTION("get from reverse cache") {
        affectsCache.insertAffects(1, std::vector<int>{2}, true);
        affectsCache.insertAffects(3, std::vector<int>{3, 4}, true);

        REQUIRE(affectsCache.getAffectedStmt(1, true).size() == 1);
        REQUIRE(affectsCache.getAffectedStmt(3, true).size() == 2);
    }
}