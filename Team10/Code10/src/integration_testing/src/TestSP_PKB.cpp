#include "SPA.h"
#include "catch.hpp"
#include "pkb/WriteFacade.h"
#include "pkb/relationship_managers/Follow.h"
#include "pkb/relationship_managers/ModifiesS.h"
#include "pkb/relationship_managers/Parent.h"
#include "pkb/relationship_managers/UsesS.h"
#include "sp/DesignExtractor.h"
#include "sp/Tokenizer.h"

TEST_CASE("Modifies and uses storage") {
    SECTION("Assign and print/read stmts") {
        std::string input = "procedure main { read y; x=z+3; print c;}";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        std::vector<std::string> res = storageManager->getModifies(1);
        REQUIRE(res.size() == 1);
        res = storageManager->getModifies(2);
        REQUIRE(res.size() == 1);

        res = storageManager->getUses(2);
        REQUIRE(res.size() == 1);
        res = storageManager->getUses(3);
        REQUIRE(res.size() == 1);
    }

    SECTION("Container statements") {
        std::string input =
            "procedure main { if(y==2) then {print x; read c; d = f+g;} else {e = "
            "2;}}";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        std::vector<std::string> res = storageManager->getModifies(1);

        REQUIRE(res.size() == 3);
        res = storageManager->getModifies(3);
        REQUIRE(res.size() == 1);
        res = storageManager->getModifies(4);
        REQUIRE(res.size() == 1);
        res = storageManager->getModifies(5);
        REQUIRE(res.size() == 1);

        res = storageManager->getUses(1);
        REQUIRE(res.size() == 4);
        res = storageManager->getUses(2);
        REQUIRE(res.size() == 1);
        res = storageManager->getUses(4);
        REQUIRE(res.size() == 2);
    }

    SECTION("nested if stmt") {
        std::string input =
            "procedure main { if(r < 2) then { if(d==2) then {print m; x = 3;} "
            "else {p = 2; } read l; } else { y = 3;} read z; } ";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        std::vector<std::string> res = storageManager->getModifies(1);
        REQUIRE(res.size() == 4);
        res = storageManager->getModifies(2);
        REQUIRE(res.size() == 2);

        res = storageManager->getUses(1);
        REQUIRE(res.size() == 3);
        res = storageManager->getUses(2);
        REQUIRE(res.size() == 2);
    }
}

TEST_CASE("Follows and parent storage") {
    SECTION("Assign and print/read stmts") {
        std::string input = "procedure main { read y; x=3; print x;}";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        std::shared_ptr<ReadFacade> readFacade = spa.getReadFacade();

        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        bool hasFollows = storageManager->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = readFacade->getFollows(1, false);
        REQUIRE(followsRs[0] == 2);

        followsRs = readFacade->getFollows(2, false);
        REQUIRE(followsRs[0] == 3);

        bool hasParents = storageManager->hasParents();
        REQUIRE(!hasParents);
        bool hasParentsStar = storageManager->hasParentsStar();
        REQUIRE(!hasParentsStar);
    }

    SECTION("If statements") {
        std::string input =
            "procedure main { if(y==2) then {print x; x = 3;} else {x = 2;}}";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        std::shared_ptr<ReadFacade> readFacade = spa.getReadFacade();

        const std::shared_ptr<Storage> storageManager = spa.getStorage();
        bool hasFollows = storageManager->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = readFacade->getFollows(2, false);
        REQUIRE(followsRs[0] == 3);

        bool hasParents = storageManager->hasParents();
        REQUIRE(hasParents);
        bool hasParentsStar = storageManager->hasParentsStar();
        REQUIRE(hasParentsStar);

        std::vector<int> parentRs = readFacade->getParents(1, false);
        REQUIRE(parentRs[0] == 2);
        REQUIRE(parentRs[1] == 3);
        REQUIRE(parentRs[2] == 4);
    }

    SECTION("while statements") {
        std::string input =
            "procedure main { while (x == 0) {print x; x = 3; read y;}} ";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        std::shared_ptr<ReadFacade> readFacade = spa.getReadFacade();

        const std::shared_ptr<Storage> storageManager = spa.getStorage();
        bool hasFollows = storageManager->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = readFacade->getFollows(2, false);
        REQUIRE(followsRs[0] == 3);

        followsRs = readFacade->getFollows(3, false);
        REQUIRE(followsRs[0] == 4);

        bool hasParents = storageManager->hasParents();
        REQUIRE(hasParents);
        bool hasParentsStar = storageManager->hasParentsStar();
        REQUIRE(hasParentsStar);

        std::vector<int> parentRs = readFacade->getParents(1, false);
        REQUIRE(parentRs[0] == 2);
        REQUIRE(parentRs[1] == 3);
        REQUIRE(parentRs[2] == 4);
    }

    SECTION("if stmt followed by assign stmt") {
        std::string input =
            "procedure main { if(y==2) then {print x; x = 3;} else {x = 2; read "
            "x;} y = 3;} ";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        std::shared_ptr<ReadFacade> readFacade = spa.getReadFacade();
        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        bool hasFollows = storageManager->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = readFacade->getFollows(1, false);
        REQUIRE(followsRs[0] == 6);

        followsRs = readFacade->getFollows(2, false);
        REQUIRE(followsRs[0] == 3);

        followsRs = readFacade->getFollows(4, false);
        REQUIRE(followsRs[0] == 5);

        bool hasParents = storageManager->hasParents();
        REQUIRE(hasParents);
        bool hasParentsStar = storageManager->hasParentsStar();
        REQUIRE(hasParentsStar);

        std::vector<int> parentRs = readFacade->getParents(1, false);
        REQUIRE(parentRs[0] == 2);
        REQUIRE(parentRs[1] == 3);
        REQUIRE(parentRs[2] == 4);
    }

    SECTION("nested if stmt followed by assign stmt") {
        std::string input =
            "procedure main { if(x < 2) then { if(y==2) then {print x; x = 3;} "
            "else {x = 2; } read x; } else { y = 3;} read y; } ";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        std::shared_ptr<ReadFacade> readFacade = spa.getReadFacade();
        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        bool hasFollows = storageManager->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = readFacade->getFollows(1, false);
        REQUIRE(followsRs[0] == 8);

        followsRs = readFacade->getFollows(2, false);
        REQUIRE(followsRs[0] == 6);

        followsRs = readFacade->getFollows(3, false);
        REQUIRE(followsRs[0] == 4);

        bool hasParents = storageManager->hasParents();
        REQUIRE(hasParents);
        bool hasParentsStar = storageManager->hasParentsStar();
        REQUIRE(hasParentsStar);

        std::vector<int> parentRs = readFacade->getParents(1, false);
        REQUIRE(parentRs[0] == 2);
        REQUIRE(parentRs[1] == 6);
        REQUIRE(parentRs[2] == 7);

        parentRs = readFacade->getParents(2, false);
        REQUIRE(parentRs[0] == 3);
        REQUIRE(parentRs[1] == 4);
        REQUIRE(parentRs[2] == 5);
    }

    SECTION("nested while and if stmt followed by assign stmt") {
        std::string input =
            "procedure main { while(x<2) { if (x==1) then { print x; } else { read "
            "x; } x=3; } x = 4; } ";
        SPA spa;
        spa.loadStringCode(input);
        spa.SPAparse();

        std::shared_ptr<ReadFacade> readFacade = spa.getReadFacade();
        const std::shared_ptr<Storage> storageManager = spa.getStorage();

        bool hasFollows = storageManager->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = readFacade->getFollows(1, false);
        REQUIRE(followsRs[0] == 6);

        followsRs = readFacade->getFollows(2, false);
        REQUIRE(followsRs[0] == 5);

        bool hasParents = storageManager->hasParents();
        REQUIRE(hasParents);
        bool hasParentsStar = storageManager->hasParentsStar();
        REQUIRE(hasParentsStar);

        std::vector<int> parentRs = readFacade->getParents(1, false);
        REQUIRE(parentRs[0] == 2);
        REQUIRE(parentRs[1] == 5);

        parentRs = readFacade->getParents(2, false);
        REQUIRE(parentRs[0] == 3);
        REQUIRE(parentRs[1] == 4);
    }
}